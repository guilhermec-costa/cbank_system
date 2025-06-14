#include "auth.h"

#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "store.h"
#include "utils.h"

extern Stores stores;

bool try_login(AuthCredentials c) {
  BankUser user = get_user_by_account_id(c.account_id);
  if (strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) return false;

  const char* ck_pwd_hash = hash_str(c.password, PWD_HASH_SALT);
  if (strcmp(ck_pwd_hash, user.password) != 0) return false;

  return true;
}

AuthCredentials make_in_mem_user(const char* acc_id, const char* pwd) {
  AuthCredentials c;
  strcpy(c.account_id, acc_id);
  strcpy(c.password, pwd);
  return c;
}

bool check_existing_user(const char* const email) {
  BankUser user = get_user_by_email(email);
  if (strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) return false;
  return true;
}

void create_user(CreateUserDTO payload) {
  /* const bool existing_user = check_existing_user(payload.email); */
  /* if (existing_user) { */
  /*   printf("Could not create user. User already exists"); */
  /*   return; */
  /* } */

  const char* const pwd_hash = hash_str(payload.password, PWD_HASH_SALT);
  mov_store_cursor(stores.user_store.store_name, END);

  const char* acc_id = gen_acc_id();
  const int next_user_id = NEXT_USER_IDENTITY;

  const struct tm at_now = now();
  char date_buf[100];
  const char* formatted_now = fmt_date(date_buf, sizeof(date_buf), at_now);

  fprintf(stores.user_store.storage,
          "id=%d;acc_id=%s;pwd=%s;created_at=%s;updated_at=%s\n",
          next_user_id, acc_id, pwd_hash, formatted_now, "NULL");

  fflush(stores.user_store.storage);
  printf("Your account has been created!");
};

BankUser register_user_form() {
  printf("Register User Form");
  BankUser user;
  RESET_ENTITY(user);

  return user;
}
