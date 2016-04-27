#ifndef __ZOOM_MEM_T__
#define __ZOOM_MEM_T__

#include <stdint.h>

#define  DEFAULT_MALLOC_SIZE ( 128 * 1024 )	//默认申请空间大小

typedef struct __mem_t{
	int total_len;
	int current_pos;
	int current_total;
	uint8_t *content;
}mem_t;

extern int zoom_initmem(mem_t *mem, int total_len);
extern int zoom_malloc(mem_t *mem);
extern int zoom_relloc(mem_t *mem, int size);

#endif