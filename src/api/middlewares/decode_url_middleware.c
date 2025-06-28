#define _POSIX_C_SOURCE 200809L
#include "../../server/http_parser.h"
#include "middlewares.h"

#include <stdlib.h>

void decode_url_middleware(struct HttpRequest* req, struct HttpResponse* res) {
  if (strlen(req->body) == 0)
    return;

  char* char_token;
  char* body_query_params_rest = req->body;
  char* url_query_params_rest  = strstr(req->path, "?");

  while ((char_token = strtok_r(body_query_params_rest, "&;", &body_query_params_rest))) {
    char* eq = strchr(char_token, '=');
    if (eq) {
      *eq         = '\0';
      char* key   = char_token;
      char* value = eq + 1;

      size_t value_len   = strlen(value);
      char*  decoded_val = malloc(value_len + 1); // +1 to \0
      memset(decoded_val, 0, value_len + 1);
      if (decoded_val) {
        char* dyn_key = strdup(key);
        if(dyn_key) {
          urldecode(decoded_val, value);
          append_query_param(dyn_key, decoded_val, &req->body_query_params_list);
        } else {
          free(decoded_val);
        }
      }
    }
  }

  if (url_query_params_rest) {
    url_query_params_rest++; // next char after "?"
    while ((char_token = strtok_r(url_query_params_rest, "&", &url_query_params_rest))) {
      char* eq = strchr(char_token, '=');
      if (eq) {
        *eq         = '\0';
        char* key   = char_token;
        char* value = eq + 1;

        size_t value_len   = strlen(value);
        char*  decoded_val = malloc(value_len + 1);
        memset(decoded_val, 0, value_len + 1);
        if (decoded_val) {
          char* dyn_key = strdup(key);
          if(dyn_key) {
            urldecode(decoded_val, value);
            append_query_param(dyn_key, decoded_val, &req->url_query_params_list);
          } else {
            free(decoded_val);
          }
        }
      }
    }
  }
}