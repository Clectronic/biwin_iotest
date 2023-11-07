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
void update_actual_time(Latencies *lat, struct timespec start,
				struct timespec end)
{
    // 计算执行时间
	double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	if (elapsed_time > lat->max)
		lat->max = elapsed_time;
	lat->avg += elapsed_time;
	lat->count++;
	return;
}

