#include "zmem.h"
#include "zlogin.h"
#include "zcurl.h"
#include "cJSON.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 *	@params		:登录信息初始化
 *	@reuturn	:
 *	@
 *
 */
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
/*
 *	@params		:获取登录信息，并完成json解析，保存token到缓冲中，后续可以保存到文件中
 *				:暂时未考虑token的12小时生命周期
 *	@reuturn	:
 *	@
 *
 */
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
	
	curl_tranfer_post(url, token, login_str_short_str, json);
 	
 	//cjson parase
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
	
	//curl destory 
	cJSON_Delete(cjson);
	return 0;
}

/*
 *	@params		:打印登录错误信息
 *	@reuturn	:
 *	@
 *
 */
int login_print_err(login_respon_t * respon)
{
	if( respon != NULL)
	{
		fprintf(stderr, "%s%s\n", "Get Null param", __func__);
		return -1;
	}

	fprintf(stdout, "help url:%s\n", respon->url);
	fprintf(stdout, "err info:%s\n", respon->err);
	fprintf(stdout, "err detail:%s\n", respon->err);

	return 0;
}