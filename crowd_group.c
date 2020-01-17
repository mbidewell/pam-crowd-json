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

  struct json_object* rsp = get_crowd_response("crowd_group.json", &conf);

  struct json_object* groups = json_object_object_get(rsp, "groups");

  int group_len = json_object_array_length(groups);

  for(int i = 0; i < group_len; i++)
  {
    struct json_object* group = json_object_array_get_idx(groups, i);
    struct json_object* gname = json_object_object_get(group, "name");

    printf("%s\n", json_object_get_string(gname));
    json_object_put(group);
  }

}