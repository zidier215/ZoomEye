#include "zmem.h"
#include "zinfo.h"
#include "zcurl.h"
#include "zweb.h"
#include "cJSON.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 *	@params		:
 *	@reuturn	:
 *	@
 *
 */
int search_print(search_info_t *info)
{
	printf("query:%s\n",info->query);
	printf("facet:%s\n",info->facet);
	printf("pages:%d\n",info->page);
	return 0 ;
}

/*
 *	@params		:
 *	@reuturn	:
 *	@
 *
 */
int search_init(search_info_t * info , char *app)
{
	strcpy(info->logapp, app);
	return 0;
}

/*
 *	@params		:拼接字符串到查询url字符串函数
 *	@reuturn	:
 *	@
 *
 */
static int search_cat_str(char *dst,char *split ,char *name,char *value,char *valuespilt)
{
	if(dst == NULL || value == NULL || name ==NULL)
		return -1;

	strcat(dst, split);
	strcat(dst, name);
	strcat(dst, "=");
	strcat(dst, valuespilt);
	strcat(dst, value);
	strcat(dst, valuespilt);

	return 0;
}

/*
 *	@params		:拼接整型到查询url字符串函数
 *	@reuturn	:
 *	@
 *
 */
static int search_cat_int(char *dst, char *split , char *name, int value, char *valuespilt)
{
	char buf[256]="";
	if(dst == NULL || name ==NULL)
		return -1;
	
	sprintf(buf, "%d",value);
	strcat(dst, split);
	strcat(dst, name);
	strcat(dst, "=");
	strcat(dst, valuespilt);
	strcat(dst, buf);
	strcat(dst, valuespilt);

	return 0;
}

/*
 *	@params		: 
 *	@reuturn	:
 *	@
 *
 */
int search_get_info(search_info_t *info, search_respon_t * respon,char *t,char *host)
{
	char url[512]="";

	if( NULL == info || NULL == respon ||  NULL == t || NULL == host )
	{
		fprintf(stderr , "Get null param check it;(func:%s)\n",__func__);
		return -1;
	}

	respon->save_result.len = 0;
	respon->save_result.total = DEFAULT_MALLOC_SIZE;
	respon->save_result.info = malloc(respon->save_result.total);
	if(respon->save_result.info == NULL)
	{
		fprintf(stderr, "Malloc buffer error,check system payload (func:%s)\n",__func__);
		return -1;
	}
	else
	{
		//清空一下缓冲区
		memset(respon->save_result.info, 0 ,respon->save_result.total); 
	}
	
	//组合请求字符串，暂时无输入有效性检查
	snprintf(url, 512, "%s%s", host, info->logapp);
	search_cat_str(url,"?","query",info->query, "\"");
	search_cat_int(url,"&","page",info->page,"");
	search_cat_str(url,"&","facet",info->facet, "");

	//http://api.zoomeye.org/host/search?query="port:21"&page=1&facet=app,os
 	curl_tranfer_get(url, &respon->save_result, login_str_callback, 1000, t);
 	
 	//cjson parase 	搜索
 	char *p = strstr(respon->save_result.info, "{");
 	if(!p)
 	{
 		return -1;
 	}
 	/*	打印原始数据	*/
 	printf("orgindata:%s\n", respon->save_result.info);

	respon->cjson = cJSON_Parse(p);
    if (!respon->cjson) 
    {
        //json解析失败释放原始数据空间，并置空
        fprintf(stderr, "cjson parser error (func:%s)\n",__func__);
        free(respon->save_result.info );
        respon->save_result.info  = NULL;

        return -1;
    }

	return 0;
}


/*
 *	@params		:句柄释放函数
 *	@reuturn	:
 *	@
 *
 */
int search_get_free(search_respon_t *info)
{

	if(info == NULL)
		return -1;	

	if(info->cjson != NULL)
	{
		cJSON_Delete(info->cjson);
	}

	if(info->save_result.info != NULL)
	{
		free(info->save_result.info);
	}
	
	return 0;
}

/*
 *	@params		: web 搜索函数
 *	@reuturn	:
 *	@
 *
 */
int search_web_init(search_info_t * info )
{
	if( NULL == info )  
	{
		return -1;
	}

	search_init(info, "/web/search");

	return 0;
}

/*
 *	@params		:host 搜索 初始化函数
 *	@reuturn	:
 *	@
 *
 */
int search_host_init(search_info_t * info )
{
	if( NULL == info )  
	{
		return -1;
	}

	strcpy(info->logapp, "/host/search");

	return 0;
}


/*
 *	@params		:web 搜索 获取json返回结果函数
 *	@reuturn	:
 *	@
 *
 */
int search_web_get_info(web_info_t *info, web_respon_t * respon,char *t,char *host)
{
		return  search_get_info( (search_info_t*)info, (search_respon_t *)respon, t, host);
}


/*
 *	@params		:web 搜索 释放句柄函数
 *	@reuturn	:
 *	@
 *
 */
int search_web_get_free(web_respon_t *info)
{
	return search_get_free(  info );
}


/*
 *	@params		:host 搜索 获取json解析结果函数
 *	@reuturn	:
 *	@
 *
 */
int search_host_get_info(host_info_t *info, host_respon_t * respon,char *t,char *host)
{
	return  search_get_info( (search_info_t*)info, (search_respon_t *)respon, t, host);
}


/*
 *	@params		:host 搜索 释放句柄函数
 *	@reuturn	:
 *	@
 *
 */
int search_host_get_free(host_respon_t *info)
{
	return search_get_free( info );
}

/*
 *	@params		:resource 搜索 释放句柄函数
 *	@reuturn	:
 *	@
 *
 */
int resource_info_init(resource_info_t * info )
{
	if( NULL == info )  
	{
		return -1;
	}

	strcpy(info->logapp, "/resources-info");

	return 0;
}

/*
 *	@params		:resource 搜索 释放句柄函数
 *	@reuturn	:
 *	@
 *
 */
int resource_info_get_info(resource_info_t *info, resource_respon_t * respon,char *t,char *host)
{
	return  search_get_info( (search_info_t*)info, (search_respon_t *)respon, t, host);
}

/*
 *	@params		:resource 搜索 释放句柄函数
 *	@reuturn	:
 *	@
 *
 */
int resource_info_get_free(resource_respon_t *info)
{
	return search_get_free( info );
}
/*------------------------------------------------------------------------------*/
/*					以下注释代码暂时啊保留，程序升级json解析时使用					*/

    //demo  {"plan": "developer", "resources": {"host-search": 9, "web-search": 40}}
 	/*json_value = cJSON_GetObjectItem(cjson, "matches");
    if (json_value !=NULL && json_value->type == cJSON_String)
    {
       	//sprintf(respon->query, "%s",json_value->valuestring);
       	printf("%d\n", cJSON_GetArraySize(json_value));
 	}
 	else if(json_value != NULL)
 	{
 		printf("matches type:%d\n", json_value->type);
 		json_value = cJSON_GetObjectItem(cjson, "facets");
	    if (json_value !=NULL && json_value->type == cJSON_String)
	    {
	       	sprintf(respon->facet, "%s",json_value->valuestring);
	 	}
	 	
	 	json_value = cJSON_GetObjectItem(cjson, "total");
	    if (json_value !=NULL && json_value->type == cJSON_Number)
	    {
	       	sprintf(respon->total, "%d",json_value->valueint);
	 	}

 		printf("%d\n", cJSON_GetArraySize(json_value));
 		
 	}
 	else if(json_value == NULL)
 	{
		json_value = cJSON_GetObjectItem(cjson, "url");
	    if (json_value !=NULL && json_value->type == cJSON_String)
	    {
	       	sprintf(respon->url, "%s",json_value->valuestring);
	 	}

		json_value = cJSON_GetObjectItem(cjson, "error");
	    if (json_value !=NULL && json_value->type == cJSON_String)
	    {
	       	sprintf(respon->err, "%s",json_value->valuestring);
	 	}
	 	
		json_value = cJSON_GetObjectItem(cjson, "message");
	    if (json_value !=NULL && json_value->type == cJSON_String)
	    {
	       	sprintf(respon->message, "%s",json_value->valuestring);
	 	}
	}*/
	//if(buffer.info)
	//	free(buffer.info);
	
