#include <string.h>
#include <stdio.h>

#include "utils.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
_handle_crowd_rest_body(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

struct json_t* get_crowd_response(const char* path, struct crowd_config* conf) 
{
    CURL *curl;
	CURLcode res;

    struct json_t *json_obj = NULL;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl)
	{
		struct curl_slist *hs = NULL;
		char *crowd_url = NULL;

        struct MemoryStruct chunk;
 
        chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
        chunk.size = 0;    /* no data at this point */ 

		crowd_url = malloc(strlen(conf->base_url) + strlen(path) + 1);
		
		hs = curl_slist_append(hs, "Content-Type: application/json");

		sprintf(crowd_url, "%s/%s", conf->base_url, path);

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
		curl_easy_setopt(curl, CURLOPT_USERNAME, conf->application);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, conf->password);
		curl_easy_setopt(curl, CURLOPT_URL, crowd_url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _handle_crowd_rest_body);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		if(conf->client_cert[0] != '\0')
		{
			curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, conf->client_cert_type);
			curl_easy_setopt(curl, CURLOPT_SSLCERT, conf->client_cert);
		}
		if(conf->client_cert_key[0] != '\0')
		{
			curl_easy_setopt(curl, CURLOPT_SSLKEY, conf->client_cert_key);
		}
		if(conf->client_cert_pwd[0] != '\0')
		{
			curl_easy_setopt(curl, CURLOPT_KEYPASSWD, conf->client_cert_pwd);
		}
		
		res = curl_easy_perform(curl);
		if (res == CURLE_OK)
		{
			long http_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
			if (http_code == 200)
			{
				json_error_t error;
				json_obj = json_loads(chunk.memory, 0, &error);
			}
		}
		curl_easy_cleanup(curl);

		free(crowd_url);
        free(chunk.memory);
	}
	curl_global_cleanup();

	return json_obj;
}