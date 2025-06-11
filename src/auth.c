#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

#include "store.h"
#include "auth.h"

extern Stores stores;

bool try_login(AuthCredentials c) {
  return true;
}

AuthCredentials make_in_mem_user(const char* id, const char* pwd) {
  AuthCredentials c;
  strcpy(c.id, id);
  strcpy(c.password, pwd);
  return c;
}

bool check_existing_user(const char* const id) {
  return get_user_by_id(id) == NULL;
}

void create_user(AuthCredentials c) {
  const bool existing_user = check_existing_user(c.id);
  if(existing_user) {
    printf("Could not create user. User already exists");
    return;
  }
  const char* pwd_salt = "$6$somesalt";
  const char* const hashed_pwd = crypt(c.password, pwd_salt);
  fprintf(stores.user_store.storage, "id=%s;pwd=%s", c.id, hashed_pwd);
  printf("User created!");
};
