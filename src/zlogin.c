#include "zmem.h"
#include "zlogin.h"
#include "zcurl.h"
#include "zxml.h"
#include "cJSON.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int cmp_to_str(char *date_new, char *time_new, char *date, char *time)
{
  int year_new = 0, mon_new = 0, day_new = 0;
  int year = 0, mon = 0, day = 0;

  printf("====>%s--%s\n%s---%s#", date_new, time_new, date, time);
  sscanf(date_new, "%04d-%02d-%02d", &year_new, &mon_new, &day_new);
  sscanf(date, "%04d-%02d-%02d", &year, &mon, &day);
  printf("::::%d-%d-%d, %d-%d-%d\n", year_new, mon_new, day_new, year, mon, day);
  if(year_new > year || mon_new > mon || day_new > day)
  {
    return -1;
  }

  sscanf(time_new, "%02d:%02d:%02d", &year_new, &mon_new, &day_new);
  sscanf(time, "%02d:%02d:%02d", &year, &mon, &day);
  printf("@@@@%d-%d-%d, %d-%d-%d\n", year_new, mon_new, day_new, year, mon, day);
  if(year_new > year + 10)
  {
    return -1;
  }
  return 0;
}

int check_login_auth_time(char *xmlname, char *token)
{
  //get current time
  int  ret =0;
  char date_new[128]="";
  char time_new[128]="";
  char date_old[128]="";
  char time_old[128]="";
  
  ret = get_time_ms_str(date_new, time_new);
  if(ret == 0)
  {
    fprintf(stdout ,"exit:%s:%d\n",__func__, __LINE__);
    goto ERROR_EXIT;
  }
  ret = get_xml_token_label("savefile", "config", date_old, time_old, token);
  if(ret != 0)
  {
    fprintf(stdout ,"exit:%s:%d\n",__func__, __LINE__);
    goto ERROR_EXIT;
  }
  ret = cmp_to_str(date_new, time_new,date_old, time_old);
  if(ret != 0)
  {
    fprintf(stdout ,"exit:%s:%d\n",__func__, __LINE__);
    goto ERROR_EXIT;
  }

  return ret;
  
ERROR_EXIT:
  return -1;
}
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
	
  int ret = 0;
	char url[512]="";
	char json[512]="";
	char token[512]="";
	cJSON *cjson =NULL, *json_value;

  ret = check_login_auth_time("savefile", token);
  if(ret == 0)
  {
      fprintf(stdout, "get token from config file \n");
      sprintf(respon->token, "%s %s","Authorization: JWT " ,token);

      return 0;
  }

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
      update_xml_all_label(AUTO_SAVE_XMLNAME , "config", json_value->valuestring);
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
