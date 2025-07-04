#ifndef CBANK_ACC_STORE_H
#define CBANK_ACC_STORE_H

#include "../data/models.h"

typedef struct {
  Account account;
  bool    success;
} GetAccountByFieldResponse;

Account                   mount_acc_from_line_buf(const char* line_buf);
const char*               acc_to_line_buf(Account* restrict acc);
GetAccountByFieldResponse get_account_by_user_id(const char* user_id);
GetAccountByFieldResponse get_account_by_field(const char* field, const char* value);

#endif /* CBANK_ACC_STORE_H */