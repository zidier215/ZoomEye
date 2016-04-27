#include "zmem.h"
#include "zinfo.h"
#include "zcurl.h"
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
int res_init(res_info_t * info)
{
	if( NULL == info )  
	{
		return -1;
	}

	strcpy(info->logapp, "/resources-info");
	return 0;
}

/*
 *	@params		:
 *	@reuturn	:
 *	@
 *
 */
int res_get_info(res_info_t *info, res_respon_t * respon,char *t,char *host)
{
	char url[512]="";
	char buffer[512]="";
	cJSON *cjson =NULL, *json_value ,* json_child;

	if( NULL == info || NULL == respon || t == NULL || host == NULL )
	{
		return -1;
	}
	
	snprintf(url, 512, "%s%s", host, info->logapp);
 	curl_tranfer_get(url, buffer, login_str_callback, 1000, t);

 	//cjson parase
 	char *p = strstr(buffer, "{");
 	if(!p)
 	{
 		return -1;
 	}

	cjson = cJSON_Parse(p);
    if (!cjson) 
    {
        fprintf(stderr, "cjson parser error\n");
        return 1;
    }

    //demo  {"plan": "developer", "resources": {"host-search": 9, "web-search": 40}}
 	json_value = cJSON_GetObjectItem(cjson, "plan");
    if (json_value !=NULL && json_value->type == cJSON_String)
    {
       	sprintf(respon->plan, "%s",json_value->valuestring);
 	}
	
	json_value = cJSON_GetObjectItem(cjson, "resources");
    if (json_value !=NULL && json_value->type == cJSON_String)
    {
       	sprintf(respon->resource, "%s",json_value->valuestring);
 	}
 	else if(json_value !=NULL && json_value->type == cJSON_Object)
 	{
 		
 		json_child = cJSON_GetObjectItem(json_value, "host-search");
	    if (json_child !=NULL && json_child->type == cJSON_Number)
	    {
	     	respon->host_search = json_child->valueint;
	 	}
	 	
	 	json_child = cJSON_GetObjectItem(json_value, "web-search");
	    if (json_child !=NULL && json_child->type == cJSON_Number)
	    {
	     	respon->web_search = json_child->valueint;
	 	}
 	}

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
 	
	cJSON_Delete(cjson);
	return 0;
}