#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "zcurl.h"
#include "cJSON.h"


/*
 *  @params     :post方法获取结果的流程函数
 *  @reuturn    :
 *  @
 *
 */

int curl_tranfer_post(char *host,void * pme, curl_callback_f f, char *json)
{
  	CURL *curl = curl_easy_init();
    
    curl_easy_setopt(curl, CURLOPT_URL, host ); 	    //设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //设置超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);          //下载数据包括HTTP头部
    //curl_easy_setopt(curl, CURLOPT_RANGE, tmpbuf);    //用于断点续传, 设置下载的分片
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, f); 	//设置下载数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pme);  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
 
    curl_easy_perform(curl);  
    curl_easy_cleanup(curl);

    //此时网站HTTP头信息已经存放在buffer内
    return 0;
 }

/*
 *  @params     :get方法获取结果的流程函数
 *  @reuturn    :
 *  @
 *
 */
int curl_tranfer_get(char *host, void * pme, curl_callback_f f,unsigned int times, char *token)
{
	char tmpbuf[1024]="";
	struct curl_slist *chunk = NULL;
 
	sprintf(tmpbuf, "0-%d\n", times);
  	
  	CURL *curl = curl_easy_init();
   
    if(curl == NULL || pme == NULL || f == NULL )
   	{	
        return -1;
    }
    /* Remove a header curl would otherwise add by itself */ 
    chunk = curl_slist_append(chunk, token);
  	
  	/* set our custom set of headers */ 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

    curl_easy_setopt(curl, CURLOPT_URL, host ); 		//设置下载的URI
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);        //设置超时
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);        //屏蔽其它信号
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);          //下载数据包括HTTP头部
    //curl_easy_setopt(curl, CURLOPT_RANGE, tmpbuf);    //用于断点续传, 设置下载的分片
    
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

/*
 *  @params     :有简单内存使用大小限制的回调函数，保存总数为 total数量的字节数据到缓冲区
 *              :可以增加内存管理函数，和relloc等机制，动态完成数据的保存。
 *  @reuturn    :
 *  @
 *
 */
size_t login_str_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    save_info_t *pme  =  ( save_info_t * )userp;
    if(pme->len + size * nmemb < pme->total)
    {
        strcat(pme->info, ptr);
    }
    
    return size * nmemb;    /*  必须返回这个大小, 否则只回调一次, 不清楚为何.*/
}
/*
 *  @params     : 打印并保存结果的函数，直接拼接，有危险，保证返回数据小于缓冲区大小时可用
 *  @reuturn    :
 *  @
 *
 */

size_t callback_get_info(void *ptr, size_t size, size_t nmemb, void *userp)
 {
    strcat( userp, ptr);
    printf("%s:%s",__func__,ptr);
    return size * nmemb;     /*  必须返回这个大小, 否则只回调一次, 不清楚为何.*/
 }

/*
 *  @params     : 拼接字符串到userp区域，无大小控制，有风险，保证返回数据小于缓冲区大小时可用
 *  @reuturn    :
 *  @
 *
 */
size_t login_str_short_str(void *ptr, size_t size, size_t nmemb, void *userp)
{
    strcat(userp, ptr);
    return size * nmemb;    /*  必须返回这个大小, 否则只回调一次, 不清楚为何.*/
}

/*
 *  @params     : 打印返回结果。用户url结果测试
 *  @reuturn    :
 *  @
 *
 */
size_t print_callback_str(void *ptr, size_t size, size_t nmemb, void *userp)
{
    printf("%s\n",ptr);
    return size * nmemb;
}

/*
 *  @params     : 打印json获取到的指针的类型，此函数可以改造成递归解析的函数(暂时无用)
 *  @reuturn    :   
 *  @
 *

void json_print_value(json_object *obj) {
    if(!obj) 
        return;
    json_type type=json_object_get_type(obj);
    if(type == json_type_boolean) 
    {
        if(json_object_get_boolean(obj)) 
        {
            printf("true");
        }
        else 
        {
            printf("false");
        }
    } 
    else if(type == json_type_double) 
    {
        printf("%lf",json_object_get_double(obj));
    } 
    else if(type == json_type_int) 
    {
        printf("%d",json_object_get_int(obj));
    } 
    else if(type == json_type_string) 
    {
        printf("%s",json_object_get_string(obj));
    } 
    else if(type == json_type_object) 
    {
        json_print_object(obj);
    } 
    else if(type == json_type_array) 
    {
        json_print_array(obj);
    } 
    else 
    {
        printf("ERROR");
    }
    printf(" ");
}
 */
