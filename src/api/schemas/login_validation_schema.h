#ifndef CBANK_LOGIN_VALIDATION_SCHEMA_H
#define CBANK_LOGIN_VALIDATION_SCHEMA_H

#include "../../data/models.h"
#include "../../server/http_parser.h"

#include <stdbool.h>

typedef struct {
  char cpf[CPF_DIGITS];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} LoginSchema;

typedef bool (*LoginBodyParser)(struct HttpRequest* req, LoginSchema* out_schema);

bool parse_login_json_schema(struct HttpRequest* req, LoginSchema* out_schema);
bool parse_login_xwf_urlencoded_schema(struct HttpRequest* req, LoginSchema* out_schema);
bool validate_login_schema(const LoginSchema* schema);

#endif /* CBANK_LOGIN_VALIDATION_SCHEMA_H */
