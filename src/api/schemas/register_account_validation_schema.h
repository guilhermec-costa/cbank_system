#ifndef CBANK_REGISTER_ACCOUNT_VALIDATION_SCHEMA_H
#define CBANK_REGISTER_ACCOUNT_VALIDATION_SCHEMA_H

#include "../../data/models.h"

#include <stdbool.h>

typedef struct {
  char name[100];
  char email[100];
  char cpf[CPF_DIGITS];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} RegisterAccountSchema;

typedef bool (*RegisterAccountBodyParser)(const char* body, RegisterAccountSchema* out_schema);

bool parse_register_acc_json_schema(const char* body, RegisterAccountSchema* out_schema);
bool parse_register_acc_xwf_urlencoded_schema(const char* body, RegisterAccountSchema* out_schema);
bool validate_schema(const RegisterAccountSchema* schema);

#endif /* CBANK_REGISTER_ACCOUNT_VALIDATION_SCHEMA_H */