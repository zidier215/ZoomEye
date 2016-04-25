#include "zmem.h"
#include "zinfo.h"
#include "zcurl.h"
#include "zsearch.h"
#include "cJSON.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


int search_init(search_info_t * info )
{
	if( NULL == info )  
	{
		return -1;
	}

	strcpy(info->logapp, "/web/search");

	return 0;
}
int search_cat_str(char *dst,char *split ,char *name,char *value,char *valuespilt)
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

int search_cat_int(char *dst, char *split , char *name, int value, char *valuespilt)
{
	if(dst == NULL || name ==NULL)
		return -1;
	char buf[256]="";

	sprintf(buf, "%d",value);
	strcat(dst, split);
	strcat(dst, name);
	strcat(dst, "=");
	strcat(dst, valuespilt);
	strcat(dst, buf);
	strcat(dst, valuespilt);

	return 0;
}
int search_get_info(search_info_t *info, search_respon_t * respon,char *t,char *host)
{
	if( NULL == info || NULL == respon || t == NULL || host == NULL )
	{
		return -1;
	}
	cJSON *cjson =NULL, *json_value ;
	
	char url[512]="";
	save_info_t buffer;
	buffer.len = 0;
	buffer.total = 100 * 1024 * 1024;
	buffer.info=malloc(buffer.total);

	if(buffer.info == NULL)
		return -1;
	memset(buffer.info, 0 ,buffer.total); 

	snprintf(url, 512, "%s%s", host, info->logapp);
	search_cat_str(url,"?","query",info->query, "\"");
	search_cat_int(url,"&","page",info->page,"");
	search_cat_str(url,"&","facet",info->facet, "");
	//printf("url:%s\n", url);
	//http://api.zoomeye.org/host/search?query="port:21"&page=1&facet=app,os
	//curl_tranfer_post(url, token, login_token_callback, json);
 	//printf("start =========>\n");
 	curl_tranfer_get(url, &buffer, login_str_callback, 1000, t);
	//printf("len:%ld\n%s\n", strlen(buffer.info), buffer.info);
 	//cjson parase
 	
 	char *p = strstr(buffer.info, "{");
 	if(!p)
 	{
 		return -1;
 	}
 	//printf("len:%ld\n%s\n", strlen(p), p);
	cjson = cJSON_Parse(p);
    if (!cjson) {
        fprintf(stderr, "cjson parser error\n");
        return 1;
    }

    //demo  {"plan": "developer", "resources": {"host-search": 9, "web-search": 40}}
 	json_value = cJSON_GetObjectItem(cjson, "matches");
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
	}
	if(buffer.info)
		free(buffer.info);
	
	cJSON_Delete(cjson);
	return 0;
}
