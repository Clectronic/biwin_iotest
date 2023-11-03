#include "include/tool.h"
#include "include/generic_io.h"
Arguments opt_args;
static thread_info threads;
int main(int argc, char * argv[]){
    int ret;
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    //初始化程序默认参数
    if(init_default_opt(&opt_args)){
        debug_log(opt_args.show_log, "init opt error");
        exit(-1);
    }
    //解析输入的参数
    if(parse_args(argc,argv,&opt_args)){
        debug_log(opt_args.show_log, "parse arg error");
        exit(-1);
    }
    //初始化执行线程
    init_test_thread_set(&threads, &opt_args);
    //开始执行测试任务
    ret = start_tests(&threads,opt_args);
    if(ret){
        perror("test_fail!\r\n");
        exit(-1);
    }
    
}