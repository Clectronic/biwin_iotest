#include <pthread.h>
#include <sys/types.h>
#include <stdint.h>
#include "time_info.h"
#ifndef THREAD_INFO
#define THREAD_INFO
typedef struct{
    int thread_num;
    pthread_t thread;
    pthread_attr_t thread_attr;
    char file_name[1024];
    off_t file_offset;
    uint64_t file_size;
    uint64_t ramdom_ops_count;
    uint64_t block_size;
    unsigned char *buffer;
    unsigned buffer_crc;
    int fd;
	unsigned long    blocksWritten;
	time_info          writeTimings;
	Latencies	     writeLatency;

	unsigned long    blocksRandomWritten;
	time_info          randomWriteTimings;
	Latencies	     randomWriteLatency;

	unsigned long    blocksRead;
	time_info          readTimings;
	Latencies	     readLatency;

	unsigned long    blocksRandomRead;
	time_info          randomReadTimings;
	Latencies	     randomReadLatency;
}thread_ins;

typedef struct{
    thread_ins* threads;
    int thread_nums;
    //time
    time_info total_write_time;
    time_info total_random_write_time;
    time_info total_read_time;
    time_info total_random_read_time;
}thread_info;
#endif