#include "include/generic_io.h"
#include "include/thread_info.h"
#include "include/lib.h"
#include "include/tool.h"
int get_seq_blocks(thread_ins* t){
    return t->file_size*MBYTE / t->block_size;
}