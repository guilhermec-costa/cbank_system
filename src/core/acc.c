#include "acc.h"

#include "../data/models.h"
#include "../data/store.h"
#include "../orm/select_query.h"
#include "../utils.h"

#include <stdio.h>

ListAccountsResponse list_accounts() {
  SelectQuery* q = new_select_query();
  q->select_all(q, DB_ACCOUNT_SECTION);
  ResultSet* result = q->execute(q);

  q->destroy(q);

  if (!result->data) {
    result->free(result);
    return (ListAccountsResponse){
        .accounts   = NULL,
        .item_count = 0,
    };
  }

  Account* accounts = calloc(result->rows, sizeof(Account));
  if (!accounts)
    return (ListAccountsResponse){
        .accounts   = NULL,
        .item_count = 0,
    };

  int items_count = 0;
  for (int i = 0; i < result->rows; i++) {
    char* line_content = get_line_from_resultset(result, i);
    if (!line_content) {
      free(line_content);
      continue;
    }
    Account acc = mount_acc_from_line_buf(line_content);
    accounts[i] = acc;
    items_count++;
    free(line_content);
  }
  return (ListAccountsResponse){
      .accounts   = accounts,
      .item_count = items_count,
  };
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