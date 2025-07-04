#include "acc_store.h"

#include "store.h"

#include <string.h>

Account mount_acc_from_line_buf(const char* line_buf) {
  Account account;
  RESET_ENTITY(account);
  account.balance = 0.0;

  char* id_token         = strstr(line_buf, "id=");
  char* user_id_fk_token = strstr(line_buf, "user_id_fk=");
  char* balance_token    = strstr(line_buf, "balance=");
  char* created_at_token = strstr(line_buf, "created_at=");
  char* updated_at_token = strstr(line_buf, "updated_at=");

  if (!id_token || !user_id_fk_token || !balance_token || !updated_at_token || !created_at_token) {
    strcpy(account.id, "");
    return account;
  }

  char   __id[20];
  char   __user_id_fk[20];
  double __balance = 0.0;
  char   __created_at[sizeof(VALID_DATETIME_STR)];
  char   __updated_at[sizeof(VALID_DATETIME_STR)];

  sscanf(id_token, "id=%19[^;];", __id);
  sscanf(user_id_fk_token, "user_id_fk=%19[^;];", __user_id_fk);
  sscanf(balance_token, "balance=%lf;", &__balance);
  sscanf(created_at_token, "created_at=%19[^;];", __created_at);
  sscanf(updated_at_token, "updated_at=%19[^;];", __updated_at);

  strcpy(account.id, __id);
  strcpy(account.user_id_fk, __user_id_fk);
  account.balance = __balance;
  strcpy(account.created_at, __created_at);
  strcpy(account.updated_at, __updated_at);

  return account;
};

const char* acc_to_line_buf(Account* restrict acc) {
  static char line_buf[256];

  snprintf(line_buf, sizeof(line_buf),
           "id=%s;user_id_fk=%s;balance=%.2lf;created_at=%s;updated_at=%s\n", acc->id,
           acc->user_id_fk, acc->balance, acc->created_at, acc->updated_at);

  return line_buf;
}
