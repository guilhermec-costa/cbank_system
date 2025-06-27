#include "user.h"

#include "../data/store.h"
#include "../orm/insert_query.h"
#include "../orm/select_query.h"
#include "../utils.h"
#include "acc.h"
#include "auth.h"

#include <stdio.h>
#include <string.h>

CreateUserDTO user_dto_from_register_acc_schema(RegisterAccountSchema* schema) {
  CreateUserDTO dto;
  memset(&dto, 0, sizeof(CreateUserDTO));

  strcpy(dto.cpf, schema->cpf);
  strcpy(dto.name, schema->name);
  strcpy(dto.password, schema->password);
  strcpy(dto.email, schema->email);

  return dto;
}

bool email_already_registered(const char* email) {
  SelectQuery* q = new_select_query();
  q->select(q, "email")->from(q, DB_USER_SECTION)->where(q, "email", "=", email);
  ResultSet* result = q->execute(q);
  return result->rows > 0;
}

BankUser make_new_user(CreateUserDTO payload) {
  BankUser user = {0};
  if (email_already_registered(payload.email)) {
    printf("Could not create user. Email already exists");
    return user;
  };

  const char* const pwd_hash = hash_str(payload.password, PWD_HASH_SALT);
  InsertQuery*      q        = new_insert_query();
  q->into(q, DB_USER_SECTION)
      ->set(q, "id", get_next_identity(DB_USER_SECTION))
      ->set(q, "acc_id", gen_acc_id())
      ->set(q, "name", payload.name)
      ->set(q, "cpf", payload.cpf)
      ->set(q, "email", payload.email)
      ->set(q, "pwd", pwd_hash)
      ->set(q, "created_at", get_fmt_now())
      ->set(q, "updated_at", "NULL");

  q->execute(q);
  q->destroy(q);
  updt_next_identity(stores.user_store.store_name);
  fflush(stores.user_store.storage);
  // rebuild user from line
  make_new_account(user);

  printf("Your account has been created!");
};
