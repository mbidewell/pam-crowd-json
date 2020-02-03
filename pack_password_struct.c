#include <jansson.h>
#include <nss.h>
#include <pwd.h>
#include <string.h>

// -1 Failed to parse
// -2 Buffer too small
static int
pack_passwd_struct(json_t *root, struct passwd *result, char *buffer, size_t buflen)
{
    char tmp_space[100];
    char *next_buf = buffer;
    size_t bufleft = buflen;
    
    char *home_dir;

    if (!json_is_object(root)) return -1;

  struct json_t* fname = json_object_get(root, "first-name");
  struct json_t* lname = json_object_get(root, "last-name");
  struct json_t* dname = json_object_get(root, "display-name");
  struct json_t* email = json_object_get(root, "email");
  struct json_t* uname = json_object_get(root, "name");

  int uname_len = json_string_length(uname);

    if (!json_is_string(fname)) return -1;
    if (!json_is_string(lname)) return -1;
    if (!json_is_integer(dname)) return -1;
    if (!json_is_string(email) && !json_is_null(email)) return -1;

    memset(buffer, '\0', buflen);

    if (bufleft <= json_string_length(fname)) return -2;
    result->pw_name = strncpy(next_buf, json_string_value(fname), bufleft);
    next_buf += strlen(result->pw_name) + 1;
    bufleft  -= strlen(result->pw_name) + 1;

    if (bufleft <= strlen("x")) return -2;
    result->pw_passwd = strncpy(next_buf, "x", bufleft);
    next_buf += strlen(result->pw_passwd) + 1;
    bufleft  -= strlen(result->pw_passwd) + 1;

    // Yay, ints are so easy!
    result->pw_uid = json_integer_value(dname);
    result->pw_gid = json_integer_value(dname);

    if (json_is_null(email))
    {
        if (bufleft <= 1) return -2;
        result->pw_gecos = strncpy(next_buf, "", 1);
        next_buf += 1;
        bufleft -= 1;
    } else {
        if (bufleft <= json_string_length(email)) return -2;
        result->pw_gecos = strncpy(next_buf, json_string_value(email), bufleft);
        next_buf += strlen(result->pw_gecos) + 1;
        bufleft  -= strlen(result->pw_gecos) + 1;
    }

    sprintf(tmp_space, "/home/%s", json_string_value(uname));

    if (bufleft <= strlen(tmp_space)) return -2;
    result->pw_dir = strncpy(next_buf, tmp_space, bufleft);
    next_buf += strlen(result->pw_dir) + 1;
    bufleft  -= strlen(result->pw_dir) + 1;

    if (bufleft <= strlen("/bin/bash")) return -2;
    result->pw_shell = strncpy(next_buf, "/bin/bash", bufleft);
    next_buf += strlen(result->pw_shell) + 1;
    bufleft  -= strlen(result->pw_shell) + 1;

    return 0;
}