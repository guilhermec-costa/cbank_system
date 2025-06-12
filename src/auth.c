#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

#include "store.h"
#include "auth.h"
#include "utils.h"

extern Stores stores;

bool try_login(AuthCredentials c) {
  BankUser user = get_user_by_id(c.id);
  if(strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) return false;

  const char* ck_pwd_hash = hash_str(c.password, PWD_HASH_SALT);
  const char* stored_pwd_hash = hash_str(user.password, PWD_HASH_SALT); 
  if(strcmp(ck_pwd_hash, stored_pwd_hash) != 0) return false;

  return true;
}

AuthCredentials make_in_mem_user(const char* id, const char* pwd) {
  AuthCredentials c;
  strcpy(c.id, id);
  strcpy(c.password, pwd);
  return c;
}

bool check_existing_user(const char* const id) {
  BankUser user = get_user_by_id(id);
  if(strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) return false;
  return true;
}

void create_user(AuthCredentials c) {
  const bool existing_user = check_existing_user(c.id);
  if(existing_user) {
    printf("Could not create user. User already exists");
    return;
  }

  const char* const pwd_hash = hash_str(c.password, PWD_HASH_SALT);
  fseek(stores.user_store.storage, 0, SEEK_END);
  fprintf(stores.user_store.storage, "id=%s;pwd=%s\n", c.id, pwd_hash);
  fflush(stores.user_store.storage);
  printf("Your account has been created!");
};
