#define _POSIX_C_SOURCE 200809L

#include "login_validation_schema.h"

#include "schema_utils.h"

#include <stdio.h>
#include <string.h>

bool parse_login_json_schema(struct HttpRequest* req, LoginSchema* out_schema) {
  const char* body      = req->body;
  const char* cpf_token = strstr(body, "\"cpf");
  const char* pwd_token = strstr(body, "\"password");

  if (!cpf_token || !pwd_token)
    return false;

  if (!parse_json_field(cpf_token, "cpf", out_schema->cpf))
    return false;
  if (!parse_json_field(pwd_token, "password", out_schema->password))
    return false;

  return true;
};

bool parse_login_xwf_urlencoded_schema(struct HttpRequest* req, LoginSchema* out_schema) {
  QueryParamList body_query_params = req->body_query_params_list;
  if (body_query_params.params_count == 0) {
    return true;
  }

  for (int i = 0; i < body_query_params.params_count; i++) {
    const char* key   = body_query_params.params[i].key;
    const char* value = body_query_params.params[i].value;
    if (strcmp(key, "cpf") == 0) {
      snprintf(out_schema->cpf, sizeof(out_schema->cpf), "%s", value);
    };
    if (strcmp(key, "password") == 0) {
      snprintf(out_schema->password, sizeof(out_schema->password), "%s", value);
    };
  }
  return true;
};

bool validate_login_schema(const LoginSchema* schema) {
  return strlen(schema->cpf) <= 11 && strlen(schema->password) <= 6;
}