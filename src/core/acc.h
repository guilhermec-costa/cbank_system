#ifndef CBANK_ACC_H
#define CBANK_ACC_H

#include "../data/models.h"

typedef enum _Transaction_Type {
  DEPOSIT,
  WITHDRAW,
} TransactionType;

Account     mount_acc_from_line_buf(const char* line_buf);
const char* acc_to_line_buf(Account* restrict acc);

typedef struct {
  Account* accounts;
  int      item_count;
} ListAccountsResponse;

ListAccountsResponse list_accounts();
Account              make_new_account(BankUser user);

#endif /* CBANK_ACC_H */