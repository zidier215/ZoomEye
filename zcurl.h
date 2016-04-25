#ifndef __ZCURL_H__
#define __ZCURL_H__
#include <stdlib.h>
typedef size_t (* curl_callback_f)(void *ptr, size_t size, size_t nmemb, void *userp);
typedef struct __save_callback_t{
	int len;
	int total;
	char *info;
}save_info_t;
extern size_t login_str_callback(void *ptr, size_t size, size_t nmemb, void *userp);
extern size_t login_str_short_str(void *ptr, size_t size, size_t nmemb, void *userp);
extern size_t print_callback_str(void *ptr, size_t size, size_t nmemb, void *userp);
extern int curl_tranfer_post(char *host,void * pme, curl_callback_f f, char *json);
extern int curl_tranfer_get(char *host, void * pme, curl_callback_f f,unsigned int times, char *token);
#endif