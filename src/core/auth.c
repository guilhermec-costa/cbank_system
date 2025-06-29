#include "auth.h"

#include "../data/models.h"
#include "../data/user_store.h"
#include "../utils.h"

#include <crypt.h>
#include <string.h>
#include <unistd.h>

struct LoginResponse login(const char* cpf, const char* pwd) {
  GetUserByCpfResponse user = get_user_by_cpf(cpf);
  if (!user.success) {
    return (struct LoginResponse){.success = false};
  }

  const char* ck_pwd_hash = hash_str(pwd, PWD_HASH_SALT);
  if (strcmp(ck_pwd_hash, user.user.password) != 0) {
    return (struct LoginResponse){.success = false};
  }

  return (struct LoginResponse){.user = user.user, .success = true};
}