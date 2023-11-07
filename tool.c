#include "include/tool.h"
#include <sys/mman.h>
#include <unistd.h>
extern Arguments opt_args;

// 函数用于计算程序的实际运行时间
void calculate_running_time(thread_info *thread_set) {
    // 计算实际运行时间（以秒为单位
	if(opt_args.run_test[WRITE_TEST]){
		time_info *info = &thread_set->total_write_time;
		double real_time = (info->real_time_stop.tv_sec - info->real_time_start.tv_sec) +
						(info->real_time_stop.tv_usec - info->real_time_start.tv_usec) / 1e6;
		double user_time = (info->user_time_stop.tv_sec - info->user_time_start.tv_sec) +
						(info->user_time_stop.tv_usec - info->user_time_start.tv_usec) / 1e6;
		double sys_time = (info->sys_time_stop.tv_sec - info->sys_time_start.tv_sec) +
						(info->sys_time_stop.tv_usec - info->sys_time_start.tv_usec) / 1e6;
		// 打印实际运行时间
		printf("WRITE_TIME_STATICS\n");
		printf("Real Time: %.6f seconds\n", real_time);
		//printf("User_Space Time: %.6f seconds\n", user_time);
		//printf("System_Space Time: %.6f seconds\n", sys_time);
	}
	if(opt_args.run_test[READ_TEST]){
		time_info *info = &thread_set->total_read_time;
		double real_time = (info->real_time_stop.tv_sec - info->real_time_start.tv_sec) +
						(info->real_time_stop.tv_usec - info->real_time_start.tv_usec) / 1e6;
		double user_time = (info->user_time_stop.tv_sec - info->user_time_start.tv_sec) +
						(info->user_time_stop.tv_usec - info->user_time_start.tv_usec) / 1e6;
		double sys_time = (info->sys_time_stop.tv_sec - info->sys_time_start.tv_sec) +
						(info->sys_time_stop.tv_usec - info->sys_time_start.tv_usec) / 1e6;
		// 打印实际运行时间
		printf("READ_TIME_STATICS\n");
		printf("Real Time: %.6f seconds\n", real_time);
		//printf("User_Space Time: %.6f seconds\n", user_time);
		//printf("System_Space Time: %.6f seconds\n", sys_time);
	}
	if(opt_args.run_test[RANDOM_WRITE_TEST]){
		time_info *info = &thread_set->total_random_write_time;
		double real_time = (info->real_time_stop.tv_sec - info->real_time_start.tv_sec) +
						(info->real_time_stop.tv_usec - info->real_time_start.tv_usec) / 1e6;
		double user_time = (info->user_time_stop.tv_sec - info->user_time_start.tv_sec) +
						(info->user_time_stop.tv_usec - info->user_time_start.tv_usec) / 1e6;
		double sys_time = (info->sys_time_stop.tv_sec - info->sys_time_start.tv_sec) +
						(info->sys_time_stop.tv_usec - info->sys_time_start.tv_usec) / 1e6;
		// 打印实际运行时间
		printf("RANDOM_WRITE_TIME_STATICS\n");
		printf("Real Time: %.6f seconds\n", real_time);
		//printf("User_Space Time: %.6f seconds\n", user_time);
		//printf("System_Space Time: %.6f seconds\n", sys_time);
	}
	if(opt_args.run_test[RANDOM_READ_TEST]){
		time_info *info = &thread_set->total_random_read_time;
		double real_time = (info->real_time_stop.tv_sec - info->real_time_start.tv_sec) +
						(info->real_time_stop.tv_usec - info->real_time_start.tv_usec) / 1e6;
		double user_time = (info->user_time_stop.tv_sec - info->user_time_start.tv_sec) +
						(info->user_time_stop.tv_usec - info->user_time_start.tv_usec) / 1e6;
		double sys_time = (info->sys_time_stop.tv_sec - info->sys_time_start.tv_sec) +
						(info->sys_time_stop.tv_usec - info->sys_time_start.tv_usec) / 1e6;
		// 打印实际运行时间
		printf("RANDOM_READ_TIME_STATICS\n");
		printf("Real Time: %.6f seconds\n", real_time);
		//printf("User_Space Time: %.6f seconds\n", user_time);
		//printf("System_Space Time: %.6f seconds\n", sys_time);
	}
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
        arg = getopt(argc, argv, "t:f:b:r:hmsRdlcSj");
        if(arg == -1){
            break;
        }
        switch(arg){
            case 'j':
                if(parseJson(args)){
                    return FAIL;
                }
                return PASS;
            case 't':
                args->thread_nums = atoi(optarg);
                check_arg(args->thread_nums, "thread_num > 0\n");
                break;
            case 'b':
                args->block_size = atoi(optarg);
                check_arg(args->block_size, "file_size should > 0\n");
                break;
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
                break;
            default:
                fprintf(stderr, "Try ./bw_iotest -h for more information!\r\n");
                exit(-1);
                break;
        }
    }
    return PASS;
}
int init_test_thread_set(thread_info *thread_set, Arguments * args){
    //默认偏移量为0
    off_t offs = 0;
    //初始化测试线程池
    memset(thread_set, 0, sizeof(thread_info));
    thread_set->thread_nums = args->thread_nums;
    thread_set->threads = (thread_ins*)calloc(thread_set->thread_nums, sizeof(thread_ins));
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
        thread_set->threads[i].file_size = args->file_size_MB / thread_set->thread_nums;

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
        thread_set->threads[i].buffer = (unsigned char*)bw_aligned_alloc(thread_set->threads[i].block_size);
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
    }
    return PASS;
    //printf("off = %dB r_offs = %dMB\r\n",offs, r_offs/1024/1024);
}

static pthread_mutex_t time_mutex; // 互斥锁
int start_task(thread_ins *t,
               uint64_t io_ops,
               file_offset_function offset_func,
               file_io_function io_func,
               int madvice,
               mmap_io_function mmap_func,
               mmap_loc_function loc_func,
               Latencies *latency) {
    int fd = t->fd;
    int ret;
	struct timeval tv_start, tv_stop;
    struct timespec start, end;
    uint64_t blocks = (uint64_t)(t->file_size * MBYTE) / (t->block_size);
    uint64_t ori_ops = io_ops;
    int open_flags;
    unsigned long bytesize = blocks*(t->block_size);
    //如果不是原始块设备，则需要预设文件大小
    if(!opt_args.rawDevice_mode){
        ret = ftruncate64(fd,bytesize);
        if(ret != 0){
            perror("truncate file error \r\n");
            close(fd);
            exit(-1);
        }
    }
    if (opt_args.mmap_mode) {
        // 使用内存映射读写
 		unsigned long chunk_num;
		// rounds the number of mmap chunks up, basically ceiling function
		off_t num_mmap_chunks = bytesize/MMAP_CHUNK_SIZE + 1;
		for(chunk_num=0; chunk_num < num_mmap_chunks; chunk_num++)
		{
			void *file_loc = NULL;
			long this_chunk_offset = t->file_offset + chunk_num*MMAP_CHUNK_SIZE;
			long this_chunk_size = MIN(MMAP_CHUNK_SIZE, (off_t)bytesize - chunk_num*MMAP_CHUNK_SIZE);
			long this_chunk_blocks = this_chunk_size / t->block_size;
			void *current_loc = NULL;

			file_loc=mmap(NULL,this_chunk_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,
					  this_chunk_offset);
			if(file_loc == MAP_FAILED) {
				fprintf(stderr, "this_chunk_size=%ld, fd=%d, offset=" OFFSET_FORMAT
					"\n", this_chunk_size, fd, t->file_offset);
				perror("Error mapping data file");
				close(fd);
				return 0;
			}

			madvise(file_loc, this_chunk_size, madvice);
			current_loc = file_loc - t->block_size; // back-one hack for sequential case
			while(io_ops--) {
				int ret;

				current_loc = (*loc_func)(file_loc, current_loc, t);

				//gettimeofday(&tv_start, NULL);
                clock_gettime(CLOCK_MONOTONIC, &start);
				ret = mmap_func(current_loc, t);
				if(ret != 0)
					exit(ret);
				if( opt_args.sync_writing ) msync(current_loc, t->block_size, MS_SYNC);
                clock_gettime(CLOCK_MONOTONIC, &end);
				//gettimeofday(&tv_stop, NULL);
				//update_latency_info(latency, tv_start, tv_stop);
			    update_actual_time(latency,start,end);
			}

			//(*blockCount) += orig_iops; // take this out of the for loop, we don't handle errors that well

			munmap(file_loc, this_chunk_size);
		}
	} else {
        // 普通文件读写
        off_t cur_offt = t->file_offset - t->block_size;
        while (io_ops) {
            //gettimeofday(&tv_start, NULL);
            cur_offt = (*offset_func)(cur_offt, t);
            //printf("cur_ooft = %d \r\n",cur_offt);
            // 记录时间的操作，加锁
            //pthread_mutex_lock(&time_mutex);
            // 获取开始时间
            clock_gettime(CLOCK_MONOTONIC, &start);
            ret = (*io_func)(fd, cur_offt, t);
            // 获取结束时间
            clock_gettime(CLOCK_MONOTONIC, &end);
            // 解锁
            //pthread_mutex_unlock(&time_mutex);
            //gettimeofday(&tv_stop, NULL);
			update_actual_time(latency,start,end);
            if (ret == -1) {
                perror("Error: during IO \r\n");
                exit(-1);
            } else {
            }
            io_ops--;
        }
    }

clean:
    fsync(fd);
    close(fd);
    return 0;
}

/*
int start_task(thread_ins *t,
               uint64_t io_ops,
               file_offset_function offset_func,
               file_io_function io_func,
               int madvice,
               mmap_io_function mmap_func,
               mmap_loc_function loc_func) 
*/
static void start_write_test(thread_ins * thread){

    debug_log(opt_args.show_log ,"seq write start !\r\n");
    //args: thread, get_blocks_func,
    start_task(thread, get_seq_blocks(thread),get_seq_offt, do_pwrite_operate,MADV_SEQUENTIAL,do_mmap_write_operation,get_sequential_loc, &thread->writeLatency);
}
static void start_read_test(thread_ins * thread){
    debug_log(opt_args.show_log ,"seq read start !\r\n");
    // 禁用输入缓冲
    //setvbuf(thread->fd, NULL, _IONBF, 0);
    start_task(thread, get_seq_blocks(thread),get_seq_offt, do_pread_operate,MADV_SEQUENTIAL,do_mmap_read_operation,get_sequential_loc, &thread->readLatency);
}
static void start_rwrite_test(thread_ins * thread){
    debug_log(opt_args.show_log ,"radom write start !\r\n");
    start_task(thread, thread->ramdom_ops_count,get_random_offset, do_pwrite_operate,MADV_RANDOM,do_mmap_write_operation,get_random_loc, &thread->randomWriteLatency);
}
static void start_rread_test(thread_ins * thread){
    debug_log(opt_args.show_log ,"radom read start !\r\n");
    start_task(thread, thread->ramdom_ops_count,get_random_offset, do_pread_operate,MADV_RANDOM,do_mmap_read_operation,get_random_loc, &thread->randomReadLatency);
}
static func_test test_cases[] = {
	start_write_test,
    start_read_test,
    start_rwrite_test,
    start_rread_test,
};

int start_tests(thread_info *thread_set,Arguments args){
    int ret;
    int i;
    double total_time = 0 ;
    time_info *timer_write = &(thread_set->total_write_time);
    time_info *timer_read = &(thread_set->total_read_time);
    time_info *timer_ramdom_write = &(thread_set->total_random_write_time);
    time_info *timer_random_read = &(thread_set->total_random_read_time);
    if(args.run_test[WRITE_TEST]){
        ret = start_test(thread_set, WRITE_TEST, args.sequential_write_mode, timer_write);
        if(ret){
            exit(-1);
        }
        for(int i = 0 ; i < thread_set->thread_nums ; i++){
            total_time += thread_set->threads[i].writeLatency.avg;
        }
        double rate = args.file_size_MB / total_time;
    }
    if(args.run_test[READ_TEST]){
        ret = start_test(thread_set, READ_TEST, args.sequential_write_mode, timer_read);
        if(ret){
            exit(-1);
        }
    }
    if(args.run_test[RANDOM_WRITE_TEST]){
        ret = start_test(thread_set, RANDOM_WRITE_TEST, args.sequential_write_mode, timer_ramdom_write);
        if(ret){
            exit(-1);
        }
    }
    if(args.run_test[RANDOM_READ_TEST]){
        ret = start_test(thread_set, RANDOM_READ_TEST, args.sequential_write_mode, timer_random_read);
        if(ret){
            exit(-1);
        }
    }
    calculate_running_time(thread_set);
    //测试结束删除文件
    for(i = 0 ; i < thread_set->thread_nums ; i++){
        remove(thread_set->threads[i].file_name);
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

int start_test(thread_info* thread_set, int test_case, int seq, time_info* t){
    int i;
    volatile int *child_status;
    threads_data * td;
    int sync_count;
    volatile int start = 0;
    int open_flags;
    int fd;

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
        // 读写操作
        open_flags = O_RDWR;

        // 如果不是原始块设备需要创建文件
        if (!opt_args.rawDevice_mode) {
            open_flags |= O_CREAT;
        }

        // 同步磁盘和内存中的数据
        if (opt_args.sync_writing) {
            open_flags |= O_SYNC;
        }

        // 如果需要直接写入
        if (opt_args.direct_io) {
            open_flags |= __O_DIRECT;
        }
        td[i].t_instant->fd = open(td[i].t_instant->file_name, open_flags, 0600); // 0600代表文件拥有者有读写权限
        if (fd == -1) {
            fprintf(stderr, "%s : %s\n", strerror(errno), td[i].t_instant->file_name);
            return -1;
        }
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
            }
        }
        if(sync_count == thread_set->thread_nums){
            break;
        }
        sleep(1);
    }while(true);//等待测试线程完成

    if(sync_count != thread_set->thread_nums){
        fprintf(stderr, "Error : %d/%d has been started \r\n",sync_count,thread_set->thread_nums);
        start = 1;
        timer_start(t);
        wait_threads_end(thread_set);
        timer_stop(t);
    }else{
        //start = 1 代表全部线程开始执行测试任务
        start = 1;
        //等待所有线程执行完毕
        timer_start(t);
        wait_threads_end(thread_set);
        timer_stop(t);
    }
    //calculate_running_time(t);
clean:
    free((int*)child_status);
    free(td);
    return PASS;
}

int parseJson(Arguments *args){
    // 读取 JSON 文件
    FILE *file = fopen("config.json", "r");
    if (!file) {
        printf("Failed to open JSON file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *jsonString = (char *)malloc(fileSize + 1);
    fread(jsonString, 1, fileSize, file);
    fclose(file);

    // 添加 JSON 字符串的结束标志
    jsonString[fileSize] = '\0';

    // 解析 JSON 数据
    cJSON *root = cJSON_Parse(jsonString);
    free(jsonString);

    if (root == NULL) {
        printf("Failed to parse JSON.\n");
        return 1;
    }
    cJSON *file_path_item = cJSON_GetObjectItem(root, "file_path");
    cJSON *thread_nums_item = cJSON_GetObjectItem(root, "thread_nums");
    cJSON *block_size_item = cJSON_GetObjectItem(root, "block_size");
    cJSON *file_size_MB_item = cJSON_GetObjectItem(root, "file_size_MB");
    cJSON *random_ops_counts_item = cJSON_GetObjectItem(root, "random_ops_counts");
    cJSON *sequential_write_mode_item = cJSON_GetObjectItem(root, "sequential_write_mode");
    cJSON *mmap_mode_item = cJSON_GetObjectItem(root, "mmap_mode");
    cJSON *rawDevice_mode_item = cJSON_GetObjectItem(root, "rawDevice_mode");
    cJSON *show_log_item = cJSON_GetObjectItem(root, "show_log");
    cJSON *sync_writing_item = cJSON_GetObjectItem(root, "sync_writing");
    cJSON *run_test_item = cJSON_GetObjectItem(root, "run_test");
    cJSON *check_data_item = cJSON_GetObjectItem(root, "check_data");
    cJSON *direct_io_item = cJSON_GetObjectItem(root, "direct_io");

    if (file_path_item && thread_nums_item && block_size_item && file_size_MB_item &&
        random_ops_counts_item && sequential_write_mode_item && mmap_mode_item &&
        rawDevice_mode_item && show_log_item && sync_writing_item && run_test_item &&
        check_data_item && direct_io_item) {
        args->file_path = strdup(file_path_item->valuestring);
        args->thread_nums = (uint32_t)thread_nums_item->valueint;
        args->block_size = (uint32_t)block_size_item->valueint;
        args->file_size_MB = (uint32_t)file_size_MB_item->valueint;
        args->random_ops_counts = (uint32_t)random_ops_counts_item->valueint;
        args->sequential_write_mode = sequential_write_mode_item->valueint;
        args->mmap_mode = mmap_mode_item->valueint;
        args->rawDevice_mode = rawDevice_mode_item->valueint;
        args->show_log = show_log_item->valueint;
        args->sync_writing = sync_writing_item->valueint;

        if (run_test_item->type == cJSON_Array) {
            for (int i = 0; i < DEFAULT_TESTCOUNT; i++) {
                cJSON *test_item = cJSON_GetArrayItem(run_test_item, i);
                args->run_test[i] = test_item->valueint;
            }
        }

        args->check_data = check_data_item->valueint;
        args->direct_io = direct_io_item->valueint;
    } else {
        printf("JSON data is incomplete or in the wrong format.\n");
        cJSON_Delete(root);
        return 1;
    }

    // 打印配置结果
    printf("Configuration:\n");
    printf("File Path: %s\n", args->file_path);
    printf("Thread Count: %u\n", args->thread_nums);
    printf("Block Size: %u\n", args->block_size);
    printf("File Size (MB): %u\n", args->file_size_MB);
    printf("Random Ops Counts: %u\n", args->random_ops_counts);
    printf("Sequential Write Mode: %d\n", args->sequential_write_mode);
    printf("Mmap Mode: %d\n", args->mmap_mode);
    printf("Raw Device Mode: %d\n", args->rawDevice_mode);
    printf("Show Log: %d\n", args->show_log);
    printf("Sync Writing: %d\n", args->sync_writing);
    printf("Run Test: [%d, %d, %d, %d]\n", args->run_test[0], args->run_test[1], args->run_test[2], args->run_test[3]);
    printf("Check Data: %d\n", args->check_data);
    printf("Direct I/O: %d\n", args->direct_io);
    // 释放 cJSON 对象
    cJSON_Delete(root);
    // 释放字符串内存
    free(args->file_path);
}