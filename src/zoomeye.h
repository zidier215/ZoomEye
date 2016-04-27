#ifndef __ZOOM_EYE_H__
#define __ZOOM_EYE_H__
#include "zmem.h"
#include "zlogin.h"
#include "zinfo.h"
#include "zweb.h"

enum {schema = 0, websearch,hostsearch,unkown};//目前支持的查询类型的枚举值

typedef struct __zoomeye_t{ /*	利用sdk时可以在此定义需要的结构体	*/
	char host[256];				//默认未http://api/zoomeye.org
	int type;					//查询类型结构体，决定使用哪些内部变量，及调用那些函数
	
	search_info_t 	search_info;//命令行输入信息保存结构体，临时存储
	login_info_t 	loginfo;	//登录信息结构体保存
	login_respon_t 	respon;		//登录信息返回信息，解析信息保存结构体

	resource_info_t resinfo;	//resouce-info初始化信息保存结构体
	resource_respon_t resrespon;//resouce-info返回结果信息，解析结果保存

	web_info_t    webinfo;		//web/search初始化信息保存结构体
	web_respon_t  webrespon;	//web/search返回结果信息，解析结果保存
	
	host_info_t   hostinfo;		//host/searchc初始化信息保存结构体
	host_respon_t hostrespon;	//host/search返回结果信息，解析结果保存
}zoomeye_t;

#endif
