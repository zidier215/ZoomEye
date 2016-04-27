#ifndef __ZSEARCH_H__
#define __ZSEARCH_H__
#include "cJSON.h"
#include "zcurl.h"
typedef struct __search_info_t
{
	int  page;				/*	搜索结果有多页时，可以设定返回结果的那一页 	*/
	char logapp[256];		/*	保存网页CGI的名字	*/
	char query[256];
	char facet[256];
}search_info_t,web_info_t,host_info_t,resource_info_t;


typedef struct __search_respon_t
{
	cJSON *cjson;			/*	解析后的json数据，可以直接进行数据获取 	*/
	int  numbers;			/*	结果有多个时，可以用来保存获取到的结果个数 	*/
	save_info_t save_result;/*	保存原始报文的结构体	*/

	char err[256];
	char url[256];
	char message[256];
	char query[256];
	char facet[256];
	char total[256];
}search_respon_t , web_respon_t,host_respon_t, resource_respon_t;

typedef struct __resouce_value_t	/*	用于保存 schema json解析的数据结果	*/
{
	char plan[256];
	char resource[256];
	int host_search;
	int web_search;
}resouce_value_t;

//初始化web搜索结构体信息
extern int search_web_init(web_info_t * info );
//获取web搜索结果的返回，信息保存有两个，json数据，和原始http报文数据；
extern int search_web_get_info(web_info_t *info, web_respon_t * respon,char *t,char *host);
//释放掉结构体申请的空间
extern int search_web_get_free(web_respon_t *info);

extern int search_host_init(host_info_t * info );
extern int search_host_get_info(host_info_t *info, host_respon_t * respon,char *t,char *host);
extern int search_host_get_free(host_respon_t *info);

extern int resource_info_init(resource_info_t * info );
extern int resource_info_get_info(resource_info_t *info, resource_respon_t * respon,char *t,char *host);
extern int resource_info_get_free(resource_respon_t *info);
#endif
