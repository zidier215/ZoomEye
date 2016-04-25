#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "zcurl.h"
#include "cJSON.h"

size_t callback_get_info(void *ptr, size_t size, size_t nmemb, void *userp)
 {
    strcat( userp, ptr);
 	  printf("%s:%s",__func__,ptr);
    return size * nmemb;     //必须返回这个大小, 否则只回调一次, 不清楚为何.
 }
 // 返回http header回调函数  
static size_t header_callback( void *ptr, size_t size, size_t nmemb, void *stream )  
{  
    int len = size * nmemb;  
    int written = len;  
    printf("%s:%s",__func__,ptr);
    return written;  
} 

int curl_tranfer_post(char *host,void * pme, curl_callback_f f, char *json)
{
  	CURL *curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, host ); 			  //设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //设置超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);          //下载数据包括HTTP头部
    //curl_easy_setopt(curl, CURLOPT_RANGE, tmpbuf);      //用于断点续传, 设置下载的分片

    //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback );      
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, f); 	//设置下载数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pme);  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
 
    curl_easy_perform(curl);  
    curl_easy_cleanup(curl);

    //此时网站HTTP头信息已经存放在buffer内.
    return 0;
 }
 int curl_tranfer_get(char *host, void * pme, curl_callback_f f,unsigned int times, char *token)
{
	char tmpbuf[1024]="";
	struct curl_slist *chunk = NULL;
 
	sprintf(tmpbuf, "0-%d\n", times);
  	
  	CURL *curl = curl_easy_init();
   if(curl == NULL || pme == NULL || f == NULL )
   		return -1;
    /* Remove a header curl would otherwise add by itself */ 
    //chunk = curl_slist_append(chunk, "Authorization: JWT ");
    chunk = curl_slist_append(chunk, token);
  	
  	/* set our custom set of headers */ 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    curl_easy_setopt(curl, CURLOPT_URL, host ); 			//设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //设置超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);          //下载数据包括HTTP头部
    //curl_easy_setopt(curl, CURLOPT_RANGE, tmpbuf);      //用于断点续传, 设置下载的分片
    
    //curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback );   
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, f); 	//设置下载数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pme);  
  
    curl_easy_perform(curl);  
    curl_easy_cleanup(curl);
	
	/* free the custom headers */ 
    curl_slist_free_all(chunk);
    //此时网站HTTP头信息已经存放在buffer内.
    return 0 ;
 }

size_t login_str_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    //printf("%s\n",ptr);
    save_info_t *pme  =  ( save_info_t * )userp;
    if(pme->len + size * nmemb < pme->total)
    {
        strcat(pme->info, ptr);
    }
    
    
    return size * nmemb;
}
size_t login_str_short_str(void *ptr, size_t size, size_t nmemb, void *userp)
{
    //printf("%s\n",ptr);
    strcat(userp, ptr);
    return size * nmemb;
}
size_t print_callback_str(void *ptr, size_t size, size_t nmemb, void *userp)
{
    printf("%s\n",ptr);
    return size * nmemb;
}
/*
void json_print_value(json_object *obj) {
    if(!obj) return;
    json_type type=json_object_get_type(obj);
    if(type == json_type_boolean) {
        if(json_object_get_boolean(obj)) {
            printf("true");
        } else {
            printf("false");
        }
    } else if(type == json_type_double) {
        printf("%lf",json_object_get_double(obj));
    } else if(type == json_type_int) {
        printf("%d",json_object_get_int(obj));
    } else if(type == json_type_string) {
        printf("%s",json_object_get_string(obj));
    } else if(type == json_type_object) {
        json_print_object(obj);
    } else if(type == json_type_array) {
        json_print_array(obj);
    } else {
        printf("ERROR");
    }
    printf(" ");
}
*/