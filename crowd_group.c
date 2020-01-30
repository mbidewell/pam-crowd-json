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

int main( int argc, char** argv) 
{
    struct crowd_config conf = {
      "http://localhost:8000"
    };

  struct json_t* rsp = get_crowd_response("crowd_group.json", &conf);

  struct json_t* groups = json_object_get(rsp, "groups");

  int group_len = json_array_size(groups);

  for(int i = 0; i < group_len; i++)
  {
    struct json_t* group = json_array_get(groups, i);
    struct json_t* gname = json_object_get(group, "name");

    printf("%s\n", json_string_value(gname));
    json_decref(group);
  }

}