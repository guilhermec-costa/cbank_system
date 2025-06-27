#include "user.h"

#include "../data/store.h"
#include "../orm/select_query.h"
#include "../utils.h"
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
  SelectQuery* q          = new_select_query();
  FILE*        user_store = get_storage_for_reading(DB_USER_SECTION);
  char         line_buf[256];

  char* email_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), user_store)) {
    line_buf[strcspn(line_buf, "\n")] = '\0';
    email_token                       = strstr(line_buf, "email=");
    if (!email_token)
      continue;

    char _tmp_email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
    sscanf(email_token, "email=%49[^;];", _tmp_email);

    if (strcmp(_tmp_email, email) == 0) {
      return true;
    }
  }
  return false;
}

BankUser make_new_user(CreateUserDTO payload) {
  BankUser   user           = {0};
  const bool existing_email = email_already_registered(payload.email);
  if (existing_email) {
    printf("Could not create user. Email already exists");
    return user;
  }

  const char* const pwd_hash = hash_str(payload.password, PWD_HASH_SALT);

  const char* acc_id       = gen_acc_id();
  const char* next_user_id = NEXT_USER_IDENTITY;

  const struct tm at_now = now();
  char            date_buf[100];
  const char*     formatted_now = fmt_date(date_buf, sizeof(date_buf), at_now);

  mov_store_cursor(stores.user_store.store_name, SEEK_END);
  fprintf(
      stores.user_store.storage,
      "id=%d;acc_id=%s;name=%s;cpf=%s;email=%s;pwd=%s;is_active=%d;created_at=%s;updated_at=%s\n",
      next_user_id, acc_id, payload.name, payload.cpf, payload.email, pwd_hash, true, formatted_now,
      "NULL");

  updt_next_identity(stores.user_store.store_name);
  fflush(stores.user_store.storage);

  mov_store_cursor(stores.account_store.store_name, SEEK_END);

  const char* next_account_identity = NEXT_ACCOUNT_IDENTITY;
  fprintf(stores.account_store.storage,
          "id=%s;user_id_fk=%s;balance=%lf;created_at=%s;updated_at=%s\n", next_account_identity,
          next_user_id, 0.0, formatted_now, "NULL");

  updt_next_identity(stores.account_store.store_name);
  fflush(stores.account_store.storage);

  printf("Your account has been created!");
};
