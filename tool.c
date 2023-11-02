#include "include/tool.h"
static inline unsigned long crc32(const void* const buffer,
		                  unsigned long     length,
		                  unsigned long     crc)
{
      const unsigned char* cp = (const unsigned char*)buffer;

      while (length--)
        crc = (crc << 8) ^ crctab[((crc >> 24) ^ *(cp++)) & 0xFF];

      return crc;
}

static void* bw_aligned_alloc(const size_t size) {
    void *memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("Error mmap()ing anonymous memory chunk");
        exit(EXIT_FAILURE);
    }
    return memory;
}

int init_default_opt(Arguments *opt_args){
    //set default_options
    opt_args->file_path = DEFAULT_DIRFILE;
    opt_args->block_size = DEFAULT_BLOCKSIZE;
    opt_args->file_size_MB = DEFAULT_FILESIZE;
    opt_args->mmap_mode = false;
    opt_args->random_ops_counts = DEFAULT_RAMOPS;
    opt_args->rawDevice_mode = false;
    opt_args->sequential_write_mode = false;
    opt_args->thread_nums = DEFAULT_THREADNUM;
    opt_args->show_log = DEFAULT_SHOWLOG;
    opt_args->sync_writing = false;
    opt_args->direct_io = false;
    for(int i = 0 ; i < DEFAULT_TESTCOUNT;i++){
        opt_args->run_test[i] = true;
    }
    return PASS;
}
void debug_log(int show, char * mesg){
    if(show){
        fprintf(stderr,"%s \r\n", mesg);
    }
}
void check_arg(int arg, char* mesg){
    if(arg < 0){
        printf(mesg);
        printf("(./bw_iotest -h) for more info! \r\n");
        exit(-1);
    }
}
static void print_option(const char* s, 
						 const char* desc, 
						 const char* def)
{
	printf("  %s          %s", s, desc);
    
	if(def)
		printf(" (default: %s)", def);

	printf("\n");
   
}
static char *my_int_to_string(int a)
{
	static char tempBuffer[128];

	sprintf(tempBuffer, "%d", a);

	return tempBuffer;
}
static void print_help_and_exit()
{

	printf("Usage: ./iotest [options]\n");

	print_option("-f", "Filesize per thread in MBytes",
				 my_int_to_string(DEFAULT_FILESIZE));

	print_option("-b", "Blocksize to use in bytes",
				 my_int_to_string(DEFAULT_BLOCKSIZE));

	print_option("-d", "Directory for test files", 
				 DEFAULT_DIRFILE);

	print_option("-t", "Number of concurrent test threads",
				 my_int_to_string(DEFAULT_THREADNUM));

	print_option("-r", "Random I/O operations per thread", 
				 my_int_to_string(DEFAULT_RAMOPS));
		     
	print_option("-R", "Use raw devices. Set device name with -d option", 0);
    
	print_option("-m", "Use mmap for I/O", 0);

	print_option("-W", "Do writing phase sequentially", 0);
	
	print_option("-S", "Do writing synchronously", 0);


	print_option("-h", "Print this help and exit", 0);

	exit(1);
}

int parse_args(int argc, char* argv[], Arguments *args){
    int arg ;
    while(1){
        arg = getopt(argc, argv, "t:f:b:r:hmsRdlcS");
        if(arg == -1){
            break;
        }
        switch(arg){
            case 't':
                args->thread_nums = atoi(optarg);
                check_arg(args->thread_nums, "thread_num > 0\n");
            case 'b':
                args->block_size = atoi(optarg);
                check_arg(args->block_size, "file_size should > 0\n");
            case 'f':
                args->file_size_MB = atoi(optarg);
                check_arg(args->file_size_MB, "block_size should > 0\n");
                break;
            case 'm':
                args->mmap_mode = true;
                break;
            case 's':
                args->sequential_write_mode = true;
                break;
            case 'S':
                args->sync_writing = true;
                break;
            case 'r':
                args->random_ops_counts = atoi(optarg);
                check_arg(args->random_ops_counts,"random_ops_counts should > 0\n");
                break;
            case 'R':
                args->rawDevice_mode = true;
                break;
            case 'l':
                args->show_log = true;
                break;
            case 'c':
                args->check_data = true;
                break;
            case 'd':
                args->direct_io = true;
                break;
            case 'h':
                 print_help_and_exit();
                 break;
            case '?':
            default:
                fprintf(stderr, "Try ./bw_iotest -h for more information!\r\n");
                exit(-1);
                break;
        }
    }
    return PASS;
}
void init_test_thread_set(thread_info *thread_set, Arguments * args){
    //默认偏移量为0
    off_t offs = 0;
    //初始化测试线程池
    memset(thread_set, 0, sizeof(thread_info));
    thread_set->thread_nums = args->thread_nums;
    thread_set->threads = calloc(thread_set->thread_nums, sizeof(thread_ins));
    /*init test thread data*/
    if(args->rawDevice_mode){
        offs = (off_t)args->file_size_MB * MBYTE;
    }else{
        //此时每个线程会新创建一个文件进行读写
        offs = 0;
    }
    //设置线程执行任务时的属性
    for(int i = 0 ; i < thread_set->thread_nums ; i++){
        thread_set->threads[i].thread_num = i;
        thread_set->threads[i].block_size = args->block_size;
        thread_set->threads[i].ramdom_ops_count = args->random_ops_counts;
        thread_set->threads[i].file_size = args->file_size_MB;
        if(args->rawDevice_mode){
            //设置每个线程对同一个原始块设备不同的偏移量
            thread_set->threads[i].file_offset = offs;
            //原始块设备打开的是同一个文件
            sprintf(thread_set->threads[i].file_name,"%s",args->file_path);
        }else{
            //设置每个线程的初始偏移量为0
            thread_set->threads[i].file_offset = 0;
            sprintf(thread_set->threads[i].file_name,"regular_file:%d",i);
        }
        //初始化线程属性
        pthread_attr_init(&(thread_set->threads[i].thread_attr));
        pthread_attr_setscope(&(thread_set->threads[i].thread_attr),PTHREAD_SCOPE_SYSTEM);
        //初始化块buffer
        thread_set->threads[i].buffer = bw_aligned_alloc(thread_set->threads[i].block_size);
        //如果使能了校验功能
        if(args->check_data){
            int j;
            const unsigned long bsize = thread_set->threads[i].block_size;
            unsigned char *buffer = thread_set->threads[i].buffer;

            for(j = 0; j < bsize; j++)
            {
                buffer[j] = rand() & 0xFF;
            }

            thread_set->threads[i].buffer_crc = crc32(buffer, bsize, 0);
        }
        printf("thread_set->threads[i].buffer_crc = %u \r\n",thread_set->threads[i].buffer_crc);
    }
    //printf("off = %dB r_offs = %dMB\r\n",offs, r_offs/1024/1024);
}
int start_task(thread_ins *t, uint32_t io_ops){
    int fd;
    uint32_t blocks = (uint32_t)( t->file_size*MBYTE )/(t->block_size);
    uint64_t ori_ops = io_ops;
    int open_flags;
    //读写操作
    open_flags = O_RDWR;
    //如果不是原始块设备需要创建文件
    if(!opt_args.rawDevice_mode){
        open_flags |= O_CREAT;
    }
    //同步磁盘和内存中的数据
    if(opt_args.sync_writing){
       open_flags |= O_SYNC; 
    }
    //如果需要直接写入
    if(opt_args.direct_io){
        open_flags |= __O_DIRECT;
    }
    fd = open(t->file_name,open_flags,0600);//0600代表文件拥有者有读写权限
    printf("blocks = %d \r\n",blocks);
}
static void start_write_test(thread_ins * thread){
    debug_log(true,"seq write start !\r\n");
    //args: thread, get_blocks_func,
    start_task(thread, get_seq_blocks(thread));
}
static void start_read_test(thread_ins * thread){
}
static void start_rwrite_test(thread_ins * thread){
}
static void start_rread_test(thread_ins * thread){
}
static func_test test_cases[] = {
	start_write_test,
    start_read_test,
    start_rwrite_test,
    start_rread_test,
};

int start_tests(thread_info *thread_set,Arguments args){
    int ret;
    if(args.run_test[WRITE_TEST]){
        ret = start_test(thread_set, WRITE_TEST, args.sequential_write_mode);
        if(ret){
            exit(-1);
        }
    }
    return PASS;
}
static void wait_threads_end(thread_info* thread_set){
    int i ;
    for(i = 0 ; i < thread_set->thread_nums ; i++){
        pthread_join(thread_set->threads[i].thread, NULL);
    }
}
static void *start_func(void * tdata){
    threads_data * td = (threads_data*)tdata;
    *td->child_status = getpid();
    if(td->pstart != NULL){
        while(*td->pstart == 0 )
            sleep(0);
    }
    td->fnc(td->t_instant);
}

int start_test(thread_info* thread_set, int test_case, int seq){
    int i;
    volatile int *child_status;
    threads_data * td;
    int sync_count;
    volatile int start = 0;
    child_status = (volatile int*)calloc(thread_set->thread_nums, sizeof(int));
    if(child_status == NULL){
        perror("Error: No enough memory for child_status callocing! \r\n");
        exit(-1);
    }
    td = (threads_data*)calloc(thread_set->thread_nums,sizeof(threads_data));
    if(td == NULL){
        perror("Error: No enough memory for td callocing! \r\n");
        exit(-1);
    }
    
    for(i = 0 ; i < thread_set->thread_nums ; i++){
        td[i].child_status = &child_status[i];
        td[i].fnc = test_cases[test_case];
        td[i].t_instant = &thread_set->threads[i];
        if(seq){
            td[i].pstart = NULL;
        }else{
            td[i].pstart = &start;
        }
        if(pthread_create(
            &(thread_set->threads[i].thread),
            &(thread_set->threads[i].thread_attr),
            start_func,
            (void*)&td[i])
        ){
            perror("Error: create pthread fail! \r\n");
            free((int*)child_status);
            free(td);
            exit(-1);
        }
        //如果是顺序写的话需要每个线程按照顺序进行写入。
        if(seq){
            debug_log(true,"seq wait pthread join ");
            pthread_join(thread_set->threads[i].thread, NULL);
        }
    }
    do{
        sync_count = 0;
        for(i = 0 ; i < thread_set->thread_nums ; i++){
            if(child_status[i]){
                sync_count++;
                printf("sync = %d\r\n",sync_count);
            }
        }
        if(sync_count == thread_set->thread_nums){
            printf("准备完成\r\n");
            break;
        }
        sleep(1);
    }while(true);//等待测试线程完成

    if(sync_count != thread_set->thread_nums){
        fprintf(stderr, "Error : %d/%d has been started \r\n",sync_count,thread_set->thread_nums);
        start = 1;
        wait_threads_end(thread_set);
    }
    //start = 1 代表全部线程开始执行测试任务
    start = 1;
    //等待所有线程执行完毕
    wait_threads_end(thread_set);
clean:
    free((int*)child_status);
    free(td);
    printf("PASS!\r\n");
    return PASS;
}

