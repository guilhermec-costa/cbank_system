#include "user.h"

#include "../data/store.h"
#include "../data/user_store.h"
#include "../orm/insert_query.h"
#include "../orm/select_query.h"
#include "../server/logger.h"
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

  q->destroy(q);
  result->free(result);
  return result->rows > 0;
}

MakeNewUserResponse make_new_user(CreateUserDTO payload) {
  BankUser user_data = {0};
  if (email_already_registered(payload.email)) {
    GLOBAL_LOGGER->log(GLOBAL_LOGGER, ERROR, "Could not create user. Email already exists");
    return (MakeNewUserResponse){.user    = user_data,
                                 .success = false,
                                 .message = "Could not create user. Email already exists"};
  };

  const char* const pwd_hash = hash_str(payload.password, PWD_HASH_SALT);
  InsertQuery*      q        = new_insert_query();
  char              next_id[50];
  get_next_identity(DB_USER_SECTION, next_id, sizeof(next_id));
  q->into(q, DB_USER_SECTION)
      ->set(q, "id", next_id)
      ->set(q, "name", payload.name)
      ->set(q, "cpf", payload.cpf)
      ->set(q, "email", payload.email)
      ->set(q, "password", pwd_hash)
      ->set(q, "is_active", "true")
      ->set(q, "created_at", get_fmt_now())
      ->set(q, "updated_at", "NULL");

  q->execute(q);
  q->destroy(q);
  updt_next_identity(stores.user_store.store_name);
  fflush(stores.user_store.storage);

  SelectQuery* select_q = new_select_query();
  select_q->select(select_q, "id,name,cpf,password,email,is_active,created_at,updated_at")
      ->from(select_q, DB_USER_SECTION)
      ->where(select_q, "id", "=", next_id);
  ResultSet* result = select_q->execute(select_q);

  select_q->destroy(select_q);

  char buf[1024] = "";
  result->full_line(result, 0, buf, sizeof(buf));
  user_data = mount_user_from_line_buf(buf);

  result->free(result);
  make_new_account(user_data);
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, INFO, "Your account has been created!");

  return (MakeNewUserResponse){.user = user_data, .success = true, .message = "User created"};
};
