#ifndef CBANK_USER_STORE_H
#define CBANK_USER_STORE_H

#include "models.h"

BankUser mount_user_from_line_buf(const char* line_buf);

typedef struct {
  BankUser user;
  bool     success;
} GetUserByCpfResponse;

GetUserByCpfResponse get_user_by_cpf(const char* cpf);

#endif /* CBANK_USER_STORE_H */
