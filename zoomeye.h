#ifndef __ZOOM_EYE_H__
#define __ZOOM_EYE_H__
#include "zmem.h"
#include "zlogin.h"
#include "zinfo.h"
#include "zsearch.h"
typedef struct __zoomeye_t{
	char host[256];
	int type;
	login_info_t 	loginfo;
	login_respon_t 	respon;
	res_info_t 		resinfo;
	res_respon_t	resrespon;
	search_info_t   search_info;
	search_respon_t webs_respon;
	search_respon_t host_respon;
}zoomeye_t;
enum {schema = 0, websearch,hostsearch,unkown};
#endif