
#ifndef __RESOURCE_IN_H__
#define __RESOURCE_IN_H__
#include "zcurl.h"
#include "cJSON.h"

typedef struct __res_info_t{
	 char logapp[256];
}res_info_t;

typedef struct __res_respon_t{
	save_info_t save_result;
	cJSON *cjson;
	
	char err[256];
	char url[256];
	char message[256];
	
	char plan[256];
	char resource[256];
	int host_search;
	int web_search;
}res_respon_t;

//查询resoure－info信息初始化
extern int res_init(res_info_t * info);

//获取resoure－info的返回值并获取剩余资源数量，保存在respon指向的空间中
extern int res_get_info(res_info_t *info, res_respon_t * respon,char *t,char *host);

#endif 