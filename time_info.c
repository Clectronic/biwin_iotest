#include "include/time_info.h"
void timer_init(time_info *t)
{
	memset( t, 0, sizeof(time_info) );
}

void timer_start(time_info *t)
{
	struct rusage ru;

	if(gettimeofday( &(t->real_time_start), NULL ))
	{
		perror("Error in timer_start from gettimeofday()\n");
		exit(10);
	}

	if(getrusage( RUSAGE_SELF, &ru ))
	{
		perror("Error in timer_start from getrusage()\n");
		exit(11);
	}

	memcpy( &(t->user_time_start), &(ru.ru_utime), sizeof( struct timeval ));
	memcpy( &(t->sys_time_start), &(ru.ru_stime), sizeof( struct timeval ));
}

void timer_stop(time_info *t)
{
	struct rusage ru;

	if( getrusage( RUSAGE_SELF, &ru ))
	{
		perror("Error in timer_stop from getrusage()\n");
		exit(11);
	}

	if(gettimeofday( &(t->real_time_stop), NULL ))
	{
		perror("Error in timer_stop from gettimeofday()\n");
		exit(10);
	}

	memcpy( &(t->user_time_stop), &(ru.ru_utime), sizeof( struct timeval ));
	memcpy( &(t->sys_time_stop), &(ru.ru_stime), sizeof( struct timeval ));
}

// 函数用于计算程序的实际运行时间
void calculate_running_time(time_info *info) {
    // 计算实际运行时间（以秒为单位）
    double real_time = (info->real_time_stop.tv_sec - info->real_time_start.tv_sec) +
                      (info->real_time_stop.tv_usec - info->real_time_start.tv_usec) / 1e6;
    double user_time = (info->user_time_stop.tv_sec - info->user_time_start.tv_sec) +
                      (info->user_time_stop.tv_usec - info->user_time_start.tv_usec) / 1e6;
    double sys_time = (info->sys_time_stop.tv_sec - info->sys_time_start.tv_sec) +
                     (info->sys_time_stop.tv_usec - info->sys_time_start.tv_usec) / 1e6;

    // 打印实际运行时间
    printf("Real Time: %.6f seconds\n", real_time);
    printf("User Time: %.6f seconds\n", user_time);
    printf("System Time: %.6f seconds\n", sys_time);
}