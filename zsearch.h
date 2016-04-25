#ifndef __ZSEARCH_H__
#define __ZSEARCH_H__
	typedef struct __search_info_t{
		char logapp[256];
		int page;
		char query[256];
		char facet[256];
	}search_info_t;

	typedef struct __search_respon_t{
		char err[256];
		char url[256];
		char message[256];
		char query[256];
		char facet[256];
		char total[256];

	}search_respon_t;

	extern int search_init(search_info_t * info );
	extern int search_get_info(search_info_t *info, search_respon_t * respon,char *t,char *host);

#endif
