#define _POSIX_C_SOURCE 200809L

#include "register_account_validation_schema.h"

#include "schema_utils.h"

#include <stdio.h>
#include <string.h>

bool parse_register_acc_json_schema(const char* body, RegisterAccountSchema* out_schema) {
  const char* cpf_token   = strstr(body, "\"cpf");
  const char* pwd_token   = strstr(body, "\"password");
  const char* name_token  = strstr(body, "\"name");
  const char* email_token = strstr(body, "\"email");

  if (!cpf_token || !pwd_token || !name_token || !email_token) {
    return false;
  }

  if (!parse_json_field(cpf_token, "cpf", out_schema->cpf))
    return false;
  if (!parse_json_field(pwd_token, "password", out_schema->password))
    return false;
  if (!parse_json_field(name_token, "name", out_schema->name))
    return false;
  if (!parse_json_field(email_token, "email", out_schema->email))
    return false;

  return true;
};

bool parse_register_acc_xwf_urlencoded_schema(const char* body, RegisterAccountSchema* out_schema) {
  char buf[256] = {0};
  snprintf(buf, sizeof(buf), "%s", body);

  char* token;
  char* rest = buf;

  while ((token = strtok_r(rest, "&", &rest))) {
    char* separator = strchr(token, '=');
    if (separator) {
      *separator        = '\0';
      const char* key   = token;
      const char* value = separator + 1;

      if (strcmp(key, "cpf") == 0) {
        snprintf(out_schema->cpf, sizeof(out_schema->cpf), "%s", value);
      };
      if (strcmp(key, "name") == 0) {
        snprintf(out_schema->name, sizeof(out_schema->name), "%s", value);
      };
      if (strcmp(key, "email") == 0) {
        snprintf(out_schema->email, sizeof(out_schema->email), "%s", value);
      };
      if (strcmp(key, "password") == 0) {
        snprintf(out_schema->password, sizeof(out_schema->password), "%s", value);
      };
    } else {
      return false;
    };
  };

  return true;
}

bool validate_schema(const RegisterAccountSchema* schema) {
  return true;
}