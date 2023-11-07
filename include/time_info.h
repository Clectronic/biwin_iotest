#ifndef TIME_INFO
#define TIME_INFO
#include <time.h>
#include <sys/resource.h>
typedef struct {
	double avg, max;
	unsigned long count, count1, count2;
} Latencies;

typedef struct{
    struct timeval real_time_start;
    struct timeval user_time_start;
    struct timeval sys_time_start;

    struct timeval real_time_stop;
    struct timeval user_time_stop;
    struct timeval sys_time_stop;
} time_info;

void timer_init(time_info *t);
void timer_start(time_info *t);
void timer_stop(time_info *t);
//void calculate_running_time(thread_info *thread_set);
void update_actual_time(Latencies *lat, struct timespec start, struct timespec end);
#endif