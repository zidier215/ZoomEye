#include "zmem.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
int zoom_initmem(mem_t *mem, int total_len)
{
	if(mem == NULL)
	{
		return -1;
	}
	mem->total_len = total_len;
	mem->current_pos = 0;
	mem->current_total = 0;
	mem->content = NULL;
	return 0;
}
//
int zoom_malloc(mem_t *mem)
{
	if(mem == NULL)
	{
		return -1;
	}
	mem->content = malloc(DEFAULT_MALLOC_SIZE);
	if(mem->content == NULL)
	{
		return -1;
	}
	return 0;
}
//
int zoom_relloc(mem_t *mem, int size)
{
	if(mem == NULL )
	{
		return -1;
	}

	if(mem->content == NULL)
	{
		return -1;
	}

	if(mem->total_len <= DEFAULT_MALLOC_SIZE || mem->total_len < (mem->current_total + size))
	{
		free(mem->content);
		mem->content = NULL;
		return -1;
	}

	mem->current_total += size;
	uint8_t * content = mem->content;

	mem->content = realloc( mem->content, mem->current_total );
	if(mem->content == NULL)
	{
		free(content);
		mem->content = NULL;
		return -1;
	}
	return 0;
}