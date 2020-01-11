#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include <curl/curl.h>

#include <json.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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

static struct MemoryStruct _get_crowd_user()
{
	char msg_buf[255];

	CURL *curl;
	CURLcode res;

    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
    chunk.size = 0;    /* no data at this point */ 


	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl)
	{
		struct curl_slist *hs = NULL;
		char *auth_url = NULL;
		char *pwd_payload = NULL;
		
		hs = curl_slist_append(hs, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/crowd_user.json");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */ 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			long http_code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

		}
		curl_easy_cleanup(curl);

	}
	curl_global_cleanup();

  return chunk;
}

int main( int argc, char** argv) 
{
  struct MemoryStruct user_json =  _get_crowd_user();

  struct json_object* user = json_tokener_parse(user_json.memory);

  free(user_json.memory);

  struct json_object* fname = json_object_object_get(user, "first-name");
  struct json_object* lname = json_object_object_get(user, "last-name");
  struct json_object* dname = json_object_object_get(user, "display-name");
  struct json_object* email = json_object_object_get(user, "email");

  printf("%s\n", json_object_get_string(fname));
  printf("%s\n", json_object_get_string(lname));
  printf("%s\n", json_object_get_string(dname));
  printf("%s\n", json_object_get_string(email));

  json_object_put(user);
  /*json_object_put(fname);
  json_object_put(lname);
  json_object_put(dname);
  json_object_put(email);*/

}