#include "register_account_validation_schema.h"

#include "schema_utils.h"

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

bool validate_schema(const RegisterAccountSchema* schema) {
  return true;
}

bool parse_register_acc_xwf_urlencoded_schema(const char* body, RegisterAccountSchema* out_schema) {

}