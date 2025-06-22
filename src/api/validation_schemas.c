#define _POSIX_C_SOURCE 200809L

#include "validation_schemas.h"

#include <stdio.h>
#include <string.h>

bool parse_login_json_schema(const char* body, LoginSchema* out_schema) {
  const char* cpf_token = strstr(body, "cpf");
  const char* pwd_token = strstr(body, "\"password\"");

  if (!cpf_token || !pwd_token)
    return false;

  if (sscanf(cpf_token, "\"cpf\" : \"%[^\"]\"", out_schema->cpf) != 1)
    return false;
  if (sscanf(cpf_token, "\"password\" : \"%[^\"]\"", out_schema->cpf) != 1)
    return false;

  return true;
};

bool parse_login_xwf_urlencoded_schema(const char* body, LoginSchema* out_schema) {
  char buf[256] = {0};
  strncpy(buf, body, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  char* token;
  char* rest = buf;

  while ((token = strtok_r(rest, "&", &rest))) {
    char* separator = strchr(token, '=');
    if (separator) {
      *separator        = '\0';
      const char* key   = token;
      const char* value = separator + 1;

      if (strcmp(key, "cpf") == 0) {
        strncpy(out_schema->cpf, value, sizeof(out_schema->cpf) - 1);
        out_schema->cpf[sizeof(out_schema->cpf) - 1] = '\0';
      };
      if (strcmp(key, "password") == 0) {
        strncpy(out_schema->password, value, sizeof(out_schema->password) - 1);
        out_schema->password[sizeof(out_schema->password) - 1] = '\0';
      };
    } else {
      return false;
    };
  };

  return true;
};

bool validate_login_schema(const LoginSchema* schema) {
  return strlen(schema->cpf) <= 11 && strlen(schema->password) <= 6;
}