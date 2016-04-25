#include "zmem.h"
#include "zlogin.h"
#include "zcurl.h"
#include "cJSON.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

size_t login_curl_callback(void *ptr, size_t size, size_t nmemb, void *userp)
 {
 	int ret = 0;
 	mem_t *mem = ( mem_t* ) userp;
 	if(mem == NULL)
 	{
 		return -1;
 	}

 	if(mem->current_pos + size * nmemb < mem->current_total)
 	{
 		strcat( (char *)(mem->content + mem->current_pos), ptr);	
 		mem->current_pos += size * nmemb;
 	}
 	else 
 	{
 		ret = zoom_relloc(mem, size * nmemb);
 		if(ret ==0)
 		{
 			strcat( (char *)(mem->content + mem->current_pos), ptr);	
 			mem->current_pos += size * nmemb;
 		}
 		else
 		{
 			fprintf(stderr, "error receive data relloc error\n");
 			return -1;//
 		}
 	}
    
    return size * nmemb;     //必须返回这个大小, 否则只回调一次, 不清楚为何.
 }

int login_init(login_info_t * info, char * username, char *password)
{
	if( NULL == info || username == NULL || password == NULL)  
	{
		return -1;
	}
	
	strcpy(info->username, username);
	strcpy(info->password, password);
	strcpy(info->logapp, "/user/login");
	return 0;
}

int login_and_get_token(login_info_t *info, login_respon_t * respon, char * host)
{
	if( NULL == info || NULL == respon)
	{
		return -1;
	}
	cJSON *cjson =NULL, *json_value;
	
	char url[512]="";
	char json[512]="";
	char token[512]="";

	snprintf(url, 512, "%s%s", host, info->logapp);
	snprintf(json, 512, "{\"username\":\"%s\",\"password\":\"%s\"}", info->username, info->password);
	//printf("url:%s, json:%s\n", url, json);
	curl_tranfer_post(url, token, login_str_short_str, json);
 	//cjson parase
 	//printf("token:%s\n", toke);
 	char *p = strstr(token, "{");
 	if(!p)
 	{
 		return -1;
 	}

	cjson = cJSON_Parse(p);
    if (!cjson) {
        fprintf(stderr, "cjson parser error\n");
        return 1;
    }
    json_value = cJSON_GetObjectItem(cjson, "url");
    if (json_value !=NULL && json_value->type == cJSON_String)
    { 	
    	sprintf(respon->err, "%s", json_value->valuestring);	
	}
    json_value = cJSON_GetObjectItem(cjson, "error");
    if (json_value !=NULL && json_value->type == cJSON_String)
    {
       	sprintf(respon->err, "%s", json_value->valuestring);
    }

    //demo {"url": "https://www.zoomeye.org/api/doc#login", "message": "missing login params username or password", "error": "bad_request"}
 	
 	json_value = cJSON_GetObjectItem(cjson, "access_token");
    if (json_value !=NULL && json_value->type == cJSON_String)
    {
       	sprintf(respon->token, "%s %s","Authorization: JWT " ,json_value->valuestring);
 	}
 	
 	json_value = cJSON_GetObjectItem(cjson, "message");
    if (json_value !=NULL && json_value->type == cJSON_String)
    {
       	sprintf(respon->message, "%s %s","Authorization: JWT " ,json_value->valuestring);
 	}
	//curl 

	cJSON_Delete(cjson);
	return 0;
}