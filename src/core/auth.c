#include "auth.h"

#include "../colorization.h"
#include "../data/models.h"
#include "../data/store.h"
#include "../utils.h"

#include <crypt.h>
#include <string.h>
#include <unistd.h>

BankUser logged_user;

struct LoginResponse login(const char* cpf, const char* pwd) {
  BankUser user = get_user_by_cpf(cpf);
  if (strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) {
    strcpy(logged_user.id, NON_EXISTING_USER_ID_FLAG);
    return (struct LoginResponse){.success = false};
  }

  const char* ck_pwd_hash = hash_str(pwd, PWD_HASH_SALT);
  if (strcmp(ck_pwd_hash, user.password) != 0)
    return (struct LoginResponse){.success = false};

  return (struct LoginResponse){.user = user, .success = false};
}

bool try_login(AuthCredentials c) {
  BankUser user = get_user_by_cpf(c.cpf);
  if (strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) {
    strcpy(logged_user.id, NON_EXISTING_USER_ID_FLAG);
    return false;
  }

  const char* ck_pwd_hash = hash_str(c.password, PWD_HASH_SALT);
  if (strcmp(ck_pwd_hash, user.password) != 0)
    return false;

  logged_user = user;
  return true;
}