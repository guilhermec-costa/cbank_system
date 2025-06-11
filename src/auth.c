#include "auth.h"

#include <crypt.h>
#include <stdio.h>
#include <string.h>

#include "store.h"

extern Stores stores;

bool try_login(AuthCredentials c) {
  printf("Id: %s", c.id);
  printf("Password: %s", c.password);
  fprintf(stores.user_store, "Id: %s\n", c.id);
};

AuthCredentials make_in_mem_user(const char* id, const char* pwd) {
  AuthCredentials c;
  strcpy(c.id, id);
  strcpy(c.password, pwd);
  return c;
}
void create_user(AuthCredentials c) {
  const char* pwd_salt = "$6$somesalt";
  const char* const hashed_pwd = crypt(c.password, pwd_salt);
  fprintf(stores.user_store, "id=%s;pwd=%s", c.id, hashed_pwd);
  printf("User created!");
};
