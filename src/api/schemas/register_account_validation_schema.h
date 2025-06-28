#ifndef CBANK_REGISTER_ACCOUNT_VALIDATION_SCHEMA_H
#define CBANK_REGISTER_ACCOUNT_VALIDATION_SCHEMA_H

#include "../../data/models.h"
#include "../../server/http_parser.h"

#include <stdbool.h>

typedef struct {
  char name[100];
  char email[100];
  char cpf[CPF_DIGITS];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} RegisterAccountSchema;

typedef bool (*RegisterAccountBodyParser)(struct HttpRequest*    req,
                                          RegisterAccountSchema* out_schema);

bool parse_register_acc_json_schema(struct HttpRequest* req, RegisterAccountSchema* out_schema);
bool parse_register_acc_xwf_urlencoded_schema(struct HttpRequest*    req,
                                              RegisterAccountSchema* out_schema);
bool validate_schema(const RegisterAccountSchema* schema);

#endif /* CBANK_REGISTER_ACCOUNT_VALIDATION_SCHEMA_H */