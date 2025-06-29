#include "acc.h"

#include "../data/models.h"
#include "../data/store.h"
#include "../orm/select_query.h"
#include "../utils.h"

#include <stdio.h>
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

Account* get_all_accounts(int* out_count) {
  FILE* acc_storage = get_storage_for_reading(DB_ACCOUNT_SECTION);
  char  f_line_buf[256];
  int   alloc_size      = 20;
  int   realloc_trigger = alloc_size - 1;

  Account* acc_ptr = (Account*)calloc(alloc_size, sizeof(Account));
  if (!acc_ptr) {
    *out_count = 0;
    return NULL;
  }

  int iter_idx = 0;

  while (fgets(f_line_buf, sizeof(f_line_buf), acc_storage)) {
    f_line_buf[strcspn(f_line_buf, "\n")] = '\0';
    Account acc_entity;
    RESET_ENTITY(acc_entity);
    if (iter_idx == realloc_trigger) {
      alloc_size += 20;
      realloc_trigger          = alloc_size - 1;
      Account* _tmp_realoc_ptr = realloc(acc_ptr, alloc_size * sizeof(Account));
      if (!_tmp_realoc_ptr) {
        free(acc_ptr);
        *out_count = 0;
        return NULL;
      }
      acc_ptr = _tmp_realoc_ptr;
    };
    acc_ptr[iter_idx++] = mount_acc_from_line_buf(f_line_buf);
  }

  fclose(acc_storage);
  *out_count = iter_idx;
  return acc_ptr;
}

#include "../orm/insert_query.h"

Account make_new_account(BankUser user) {
  Account      account = {0};
  InsertQuery* q       = new_insert_query();
  char         next_id[50];
  get_next_identity(DB_ACCOUNT_SECTION, next_id, sizeof(next_id));

  q->into(q, DB_ACCOUNT_SECTION)
      ->set(q, "id", next_id)
      ->set(q, "user_id_fk", user.id)
      ->set(q, "balance", "0.000")
      ->set(q, "created_at", get_fmt_now())
      ->set(q, "updated_at", "NULL");
  q->execute(q);

  SelectQuery* select_q = new_select_query();
  select_q->from(select_q, DB_ACCOUNT_SECTION)->where(select_q, "id", "=", next_id);
  ResultSet* result = select_q->execute(select_q);

  select_q->destroy(select_q);

  char buf[1024] = "";
  result->full_line(result, 0, buf, sizeof(buf));

  account = mount_acc_from_line_buf(buf);

  updt_next_identity(stores.account_store.store_name);
  fflush(stores.account_store.storage);
  result->free(result);
  return account;
}