#include "../../data/models.h"

#include <stdbool.h>

typedef struct {
  char cpf[CPF_DIGITS];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} LoginSchema;

typedef bool (*LoginBodyParser)(const char* body, LoginSchema* out_schema);

bool parse_login_json_schema(const char* body, LoginSchema* out_schema);
bool parse_login_xwf_urlencoded_schema(const char* body, LoginSchema* out_schema);
bool validate_login_schema(const LoginSchema* schema);