#include "tool.h"
#ifndef GENERIC_IO
#define GENERIC_IO
#define OFFSET_FORMAT "0X%Lx" 
typedef int                (*file_io_function)     (int fd, off_t offset, thread_ins *t);
typedef int                (*mmap_io_function)     (void *loc, thread_ins *t);

typedef off_t          (*file_offset_function) (off_t current_offset, thread_ins *t);
typedef void *             (*mmap_loc_function)    (void *base_loc, void *current_loc, thread_ins *t);
off_t get_seq_offt(off_t cur_offt, thread_ins *t);
int get_seq_blocks(thread_ins* t);
int do_pwrite_operate(int fd, off_t offset, thread_ins * t);
#endif