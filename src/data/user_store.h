#ifndef CBANK_USER_STORE_H
#define CBANK_USER_STORE_H

#include "models.h"

BankUser mount_user_from_line_buf(const char* line_buf);

typedef struct {
  BankUser user;
  bool     success;
} GetUserByFieldResponse;

GetUserByFieldResponse get_user_by_field(const char* field, const char* value);
GetUserByFieldResponse get_user_by_cpf(const char* cpf);
GetUserByFieldResponse get_user_by_id(const char* id);

#endif /* CBANK_USER_STORE_H */
