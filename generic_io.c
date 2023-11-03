#include "include/generic_io.h"

off_t get_seq_offt(off_t cur_offt, thread_ins *t){
    return cur_offt + t->block_size;
}
int get_seq_blocks(thread_ins* t){
    return t->file_size*MBYTE / t->block_size;
}

int do_pwrite_operate(int fd, off_t offset, thread_ins * t)
{
	ssize_t ret = pwrite( fd, t->buffer, t->block_size, offset );
	if( ret  != t->block_size ) {
		if( ret == -1 ) {
			perror("Error do_pwriting to file");
		} else {
			fprintf(stderr, "Tried to write %ld bytes from offset " OFFSET_FORMAT " of file %s of length " OFFSET_FORMAT ", but only wrote %d bytes\n", t->block_size, offset, t->file_name, t->file_size*MB, ret);
		}
		return -1;
	}

	return 0;
}