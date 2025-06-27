#include "user.h"

#include "../data/store.h"
#include "../orm/insert_query.h"
#include "../orm/select_query.h"
#include "../utils.h"
#include "acc.h"
#include "auth.h"

#include <stdio.h>
#include <string.h>

BankUser mount_user_from_line_buf(const char* line_buf) {
  BankUser user;
  memset(&user, 0, sizeof(BankUser));

  char* id_token         = strstr(line_buf, "id=");
  char* name_token       = strstr(line_buf, "name=");
  char* cpf_token        = strstr(line_buf, "cpf=");
  char* pwd_token        = strstr(line_buf, "pwd=");
  char* is_active_token  = strstr(line_buf, "is_active=");
  char* created_at_token = strstr(line_buf, "created_at=");
  char* updated_at_token = strstr(line_buf, "updated_at=");

  if (!id_token || !name_token || !cpf_token || !pwd_token || !is_active_token ||
      !updated_at_token || !created_at_token) {
    strcpy(user.id, "");
    return user;
  }

  char __id[20];
  char __name[256];
  char __cpf[20];
  char __pwd[256];
  int  __is_active = 0;
  char __created_at[20];
  char __updated_at[20];

  sscanf(id_token, "id=%19[^;];", __id);
  sscanf(name_token, "name=%255[^;];", __name);
  sscanf(cpf_token, "cpf=%19[^;];", __cpf);
  sscanf(pwd_token, "pwd=%255[^;];", __pwd);
  sscanf(is_active_token, "is_active=%d;", &__is_active);
  sscanf(created_at_token, "created_at=%19[^;];", __created_at);
  sscanf(updated_at_token, "updated_at=%19[^;];", __updated_at);

  strcpy(user.id, __id);
  strcpy(user.name, __name);
  strcpy(user.cpf, __cpf);
  strcpy(user.password, __pwd);
  user.is_active = __is_active;
  strcpy(user.created_at, __created_at);
  strcpy(user.updated_at, __updated_at);

  return user;
};

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
  BankUser user_data = {0};
  if (email_already_registered(payload.email)) {
    printf("Could not create user. Email already exists");
    return user_data;
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
      ->set(q, "pwd", pwd_hash)
      ->set(q, "created_at", get_fmt_now())
      ->set(q, "updated_at", "NULL");

  q->execute(q);
  q->destroy(q);
  updt_next_identity(stores.user_store.store_name);
  fflush(stores.user_store.storage);

  SelectQuery* select_q = new_select_query();
  select_q->select(select_q, "id,name,cpf,password,email,created_at,updated_at")
      ->from(select_q, DB_USER_SECTION)
      ->where(select_q, "id", "=", next_id);
  ResultSet* result = select_q->execute(select_q);

  select_q->destroy(select_q);

  char buf[1024] = "";
  result->full_line(result, 0, buf, sizeof(buf));
  user_data = mount_user_from_line_buf(buf);

  result->free(result);
  make_new_account(user_data);
  printf("Your account has been created!");
};
