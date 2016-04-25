
#ifndef __LOGIN_IN_H__
#define __LOGIN_IN_H__

typedef struct __login_info_t{

	 char username[256];
	 char password[256];
	 char logapp[256];
}login_info_t;

typedef struct __login_respon_t{
	char err[256];
	char url[256];
	char message[256];
	char token[512];
}login_respon_t;

extern int login_init(login_info_t * info, char * username, char *password);
extern int login_and_get_token(login_info_t *info, login_respon_t * respon,char *host);
#endif 