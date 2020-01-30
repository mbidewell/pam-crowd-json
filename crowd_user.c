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

#include <jansson.h>

#include "utils.h"
#include <nss.h>

int main( int argc, char** argv) 
{
    struct crowd_config conf = {
      "http://localhost:8000"
    };

  struct json_t* user = get_crowd_response("crowd_user.json", &conf);

  struct json_t* fname = json_object_get(user, "first-name");
  struct json_t* lname = json_object_get(user, "last-name");
  struct json_t* dname = json_object_get(user, "display-name");
  struct json_t* email = json_object_get(user, "email");

  printf("%s\n", json_string_value(fname));
  printf("%s\n", json_string_value(lname));
  printf("%s\n", json_string_value(dname));
  printf("%s\n", json_string_value(email));

  json_decref(user);

}

