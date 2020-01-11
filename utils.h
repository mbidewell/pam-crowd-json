#ifndef _UTILS_H
#define _UTILS_H


#include <curl/curl.h>
#include <json.h>

#define CFG_CROWD_BASE "crowd_base_url"
#define CFG_CROWD_APP "crowd_app"
#define CFG_CROWD_PWD "crowd_pwd"
#define CFG_CLIENT_CERT "client_cert_file"
#define CFG_CLIENT_KEY "client_key_file"
#define CFG_CLIENT_PWD "client_key_pwd"
#define CFG_CLIENT_TYPE "client_key_type"
#define CFG_FILE "/etc/nss_crowd.conf"

struct crowd_config
{
    char base_url[100];
    char application[30];
    char password[30];
    char client_cert[100];
    char client_cert_key[100];
    char client_cert_pwd[30];
    char client_cert_type[5];
};


struct json_object* get_crowd_response(const char* path, struct crowd_config* cfg);


#endif