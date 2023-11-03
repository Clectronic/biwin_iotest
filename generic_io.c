#include "include/generic_io.h"

unsigned long crc32(const void* const buffer,
		                  unsigned long     length,
		                  unsigned long     crc)
{
      const unsigned char* cp = (const unsigned char*)buffer;

      while (length--)
        crc = (crc << 8) ^ crctab[((crc >> 24) ^ *(cp++)) & 0xFF];

      return crc;
}
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

int do_pread_operate(int fd, off_t offset, thread_ins * t)
{
	ssize_t rc = pread( fd, t->buffer, t->block_size, offset );
	if( rc != t->block_size ) {
		if( rc == -1 ) {
			perror("Error do_preading to file");
		} else {
			fprintf(stderr, "Tried to read %ld bytes from offset " OFFSET_FORMAT " of file %s of length " OFFSET_FORMAT ", but only read %d bytes\n", 
							t->block_size, offset, t->file_name, t->file_size*MB, rc);
		}

		return -1;
	}
	else {
		if( opt_args.check_data )
		{
			const unsigned crc = crc32(t->buffer, t->block_size, 0);

			if(crc != t->buffer_crc)
			{
				fprintf(stderr, "Thread(%lu) consistency check failed at offset %Lu\n", t->thread_num, (long long unsigned int)offset);
				return -1;
			}
		}
	}

	return 0;
}