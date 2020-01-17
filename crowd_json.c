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

#include "utils.h"

int main( int argc, char** argv) 
{
    struct crowd_config conf = {
      "http://localhost:8000"
    };

  struct json_object* user = get_crowd_response("crowd_user.json", &conf);

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