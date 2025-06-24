#include "acc.h"

#include "../data/models.h"
#include "../data/store.h"
#include "../utils.h"

#include <stdio.h>
#include <string.h>

extern BankUser logged_user;

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
  char   __user_id_fk[ACC_ID_MAX_CHAR_CONSTRAINT];
  double __balance = 0.0;
  char   __created_at[sizeof(VALID_DATETIME_STR)];
  char   __updated_at[sizeof(VALID_DATETIME_STR)];

  sscanf(id_token, "id=%19[^;];", __id);
  sscanf(user_id_fk_token, "user_id_fk=%9[^;];", __user_id_fk);
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

const char* acc_to_json_obj(Account* restrict acc) {
  const char* json_start = "{";
  const char* json_end   = "}";
}

double check_user_balance() {
  FILE*   acc_store = get_storage_for_reading(DB_ACCOUNT_SECTION);
  Account account_entity;
  RESET_ENTITY(account_entity);
  account_entity.balance = 0.0;
  char f_line_buf[256];

  const char* user_id_fk_token = NULL;
  while (fgets(f_line_buf, sizeof(f_line_buf), acc_store)) {
    terminate_str_by_newline(f_line_buf);
    user_id_fk_token = strstr(f_line_buf, "user_id_fk=");
    if (!user_id_fk_token)
      continue;

    char __user_id_fk[20];
    sscanf(user_id_fk_token, "user_id_fk=%19[^;];", __user_id_fk);
    if (strcmp(__user_id_fk, logged_user.id) == 0) {
      account_entity = mount_acc_from_line_buf(f_line_buf);
    }
  }

  return account_entity.balance;
}

int make_transaction_op(double v, TransactionType ttype) {
  bool    updated     = false;
  FILE*   acc_storage = get_storage_for_reading(DB_ACCOUNT_SECTION);
  Account account_entity;
  RESET_ENTITY(account_entity);

  const char* tmp_f_name = "acc_tmp_db";
  FILE*       tmp_f      = fopen(tmp_f_name, "w");
  if (tmp_f == NULL) {
    printf("Failed to open temporary file for updating account.\n");
    return 1;
  }

  char        f_line_buf[256];
  const char* user_id_fk_token = NULL;
  while (fgets(f_line_buf, sizeof(f_line_buf), acc_storage)) {
    terminate_str_by_newline(f_line_buf);
    user_id_fk_token = strstr(f_line_buf, "user_id_fk=");
    if (!user_id_fk_token) {
      fputs(f_line_buf, tmp_f);
      continue;
    }

    account_entity = mount_acc_from_line_buf(f_line_buf);
    if (strcmp(account_entity.user_id_fk, logged_user.id) == 0) {
      switch (ttype) {
        case DEPOSIT: {
          account_entity.balance += v;
          break;
        }
        case WITHDRAW: {
          if (account_entity.balance < 0.000001) {
          }
          account_entity.balance -= v;
          break;
        }
      }
      updated = true;
    }
    fputs(acc_to_line_buf(&account_entity), tmp_f);
  }

  fclose(tmp_f);
  remove(DB_ACCOUNT_SECTION);
  rename(tmp_f_name, DB_ACCOUNT_SECTION);

  fclose(acc_storage);
  stores.account_store.storage = db_instance(DB_ACCOUNT_SECTION);

  if (!updated) {
    printf("⚠️  Account for user ID '%s' not found.\n", logged_user.id);
    return 1;
  }

  return 0;
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