#ifndef TOOL    
#define TOOL

#include "lib.h"
#include "thread_info.h"
#include "generic_io.h"
#include "time_info.h"
#include "cJSON.h"
#include <stdint.h>
#include <getopt.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#define MAX_PATH 64
#define KBYTE 1024
#define MBYTE KBYTE*1024
#define GBYTE MBYTE*1024
#define MMAP_CHUNK_SIZE 1073741824uLL
#define DEFAULT_BLOCKSIZE 4096
#define DEFAULT_THREADNUM 1
#define DEFAULT_FILESIZE 256
#define DEFAULT_RAMOPS 4000
#define DEFAULT_DIRFILE "."
#define DEFAULT_SHOWLOG 0
#define DEFAULT_TESTCOUNT 4


typedef struct {
    char *file_path;
    uint32_t thread_nums;
    uint32_t block_size;
    uint32_t file_size_MB;
    uint32_t random_ops_counts;
    int sequential_write_mode;
    int mmap_mode;
    int rawDevice_mode;
    int show_log;
    int sync_writing;
    int run_test[DEFAULT_TESTCOUNT];
    int check_data;
    int direct_io;
} Arguments;

void debug_log(int show, char * mesg);
int parse_args(int argc, char* argv[], Arguments *args);
int init_default_opt(Arguments *opt_args);
static char *my_int_to_string(int a);
static void print_option(const char* s, 
						 const char* desc, 
						 const char* def);
static void print_help_and_exit();

static void* bw_aligned_alloc(const size_t size);
int init_test_thread_set(thread_info *thread_set, Arguments * args);


#define WRITE_TEST 0
#define READ_TEST  1
#define RANDOM_WRITE_TEST 2
#define RANDOM_READ_TEST  3
int start_tests(thread_info *thread_set,Arguments args);
int start_test(thread_info* threads, int test_case, int seq, time_info *t);
void calculate_running_time(thread_info *thread_set);
typedef void(*func_test)(thread_ins *);
typedef struct{
  volatile int *child_status;
  func_test fnc;
  thread_ins *t_instant;
  volatile int *pstart;
} threads_data;
extern Arguments opt_args;
int parseJson(Arguments *args);
#endif