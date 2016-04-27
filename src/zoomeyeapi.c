//-------------------------------
//
//
//
//==>login api
//==>
//==>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <getopt.h>
#include "zoomeye.h"

extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

zoomeye_t eye;
/* options descriptor */
static struct option longopts[] = {
     { "username", required_argument,           NULL,           'u' },
     { "passwd",   required_argument,           NULL,           'p' },
     { "query", required_argument,              NULL,           'q' },
     { "facet", required_argument,              NULL,           'f' },
     { "page", required_argument,               NULL,           'g' },
     { NULL,         0,                         NULL,            0  }
};

void usage()
{
    fprintf(stderr, "================================================\n");
    fprintf(stderr, "|          This is for ZoomEye search          |\n");
    fprintf(stderr, "|          Usage: ./a.out(elf) options         |\n");
    fprintf(stderr, "|          -u or --username username (must)    |\n");
    fprintf(stderr, "|          -p or --passwd passwd   (must)      |\n");
    fprintf(stderr, "|          -t or --searchtype  searchtype (must)|\n");
    fprintf(stderr, "|          -q or --query query  (must)         |\n");
    fprintf(stderr, "|          -f or --facet facet                 |\n");
    fprintf(stderr, "|          -g or --page page                   |\n");
    fprintf(stderr, "================================================\n");
}

int main(int argc ,char **argv)
 {	

    int ch = 0;
    char username[256]="";
    char passwd[256]="";
    eye.type = -1;

    while ((ch = getopt_long(argc, argv, "Uu:Pp:Tt:Qq:Gg:Ff:Hh", longopts, NULL)) != -1)
        switch (ch){
            case 'U':
            case 'u':  
                strcpy(username,  argv[optind - 1]);  
                break;
            case 'P':
            case 'p':
                strcpy(passwd,    argv[optind - 1]);  
                break;
            case 'T':
            case 't':
                if(strcmp(argv[optind - 1], "schema") == 0)
                {
                    eye.type = schema;
                }
                else if(strcmp(argv[optind - 1], "web") == 0)
                {
                    eye.type = websearch;
                }
                else if(strcmp(argv[optind - 1],"host" ) == 0)
                {
                    eye.type = hostsearch;
                }
                else
                {
                    eye.type = unkown;
                }
                printf("type:%s--typeid:%d\n",argv[optind - 1], eye.type);
                break;
            case 'Q':
            case 'q':
                
                strcpy(eye.search_info.query, argv[optind - 1]);
                
                break;
            case 'F':
            case 'f':
                strcpy(eye.search_info.facet, argv[optind - 1]);
                
                break;
            case 'G':
            case 'g':
                eye.search_info.page =  atoi(argv[optind - 1]);
                
                break;
            case 'h':
            case 'H':
            default:
                usage();
                break;
        }

    argc -= optind;
    argv += optind;

    if(strlen(username)==0 || strlen(passwd) == 0)
    {
        fprintf(stderr, "Username(%s) & Passwd(%s) must pass to program\n", username, passwd);
        return -1;
    }

    strcpy(eye.host,"http://api.zoomeye.org");
    login_init(&eye.loginfo, username, passwd);
    printf("user:%s, passwd:%s\n", eye.loginfo.username, eye.loginfo.password);
    login_and_get_token(&eye.loginfo, &eye.respon, eye.host);
    printf("token:%s\nerr:%s\nurl:%s\nmessage:%s\n", eye.respon.token, eye.respon.err, eye.respon.url, eye.respon.message);
    if(strlen(eye.respon.token) == 0)
    {

        fprintf(stderr, "Get Authorization token error\n");
        fprintf(stderr, "Error: %s\n", eye.respon.err);
        fprintf(stderr, "Error Detail: %s\n", eye.respon.message);
        return -1;
    }

    switch(eye.type)
    {
        case schema:
            resource_info_init(&eye.resinfo);
            resource_info_get_info(&eye.resinfo, &eye.resrespon, eye.respon.token,eye.host);
            resource_info_get_free(&eye.resrespon);
            break;
        case websearch:
            eye.webinfo = eye.search_info;
            search_web_init(&eye.webinfo);
            search_web_get_info(&eye.webinfo, &eye.webrespon,eye.respon.token, eye.host);
            search_web_get_free(&eye.webrespon);
            break;
        case hostsearch:
            eye.hostinfo = eye.search_info;
            search_host_init(&eye.hostinfo);
            search_host_get_info(&eye.hostinfo, &eye.hostrespon, eye.respon.token, eye.host);
            search_host_get_free(&eye.hostrespon);
            break;
        default:
            fprintf(stderr, "Must Input Search type (-t web or host)\n");
            break;
    }
    return 0;
}