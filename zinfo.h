
#ifndef __RESOURCE_IN_H__
#define __RESOURCE_IN_H__

typedef struct __res_info_t{
	 char logapp[256];
}res_info_t;

typedef struct __res_respon_t{
	char err[256];
	char url[256];
	char message[256];
	char plan[256];
	char resource[256];
	int host_search;
	int web_search;
}res_respon_t;

extern int res_init(res_info_t * info);
extern int res_get_info(res_info_t *info, res_respon_t * respon,char *t,char *host);
#endif 