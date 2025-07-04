#include "acc_store.h"

#include "../orm/select_query.h"
#include "models.h"
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

GetAccountByFieldResponse get_account_by_user_id(const char* user_id) {
  return get_account_by_field("user_id_fk", user_id);
}
GetAccountByFieldResponse get_account_by_field(const char* field, const char* value) {
  Account      account = {0};
  SelectQuery* q       = new_select_query();
  q->select_all(q, DB_ACCOUNT_SECTION)->where(q, field, "=", value);
  ResultSet* result = q->execute(q);

  q->destroy(q);

  if (result->rows == 0) {
    result->free(result);
    return (GetAccountByFieldResponse){
        .account = account,
        .success = false,
    };
  };

  char buf[1024] = "";
  result->full_line(result, 0, buf, sizeof(buf));
  account = mount_acc_from_line_buf(buf);

  result->free(result);
  return (GetAccountByFieldResponse){
      .account = account,
      .success = true,
  };
}