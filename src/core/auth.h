#ifndef CBANK_AUTH_H
#define CBANK_AUTH_H

#include "../api/dtos/user_models.h"
#include "../data/models.h"

#include <stdbool.h>

#define MAX_LOGIN_TRIES 3
#define NON_EXISTING_USER_ID_FLAG ""
#define PWD_HASH_SALT "PHToXCpRR1qGC2G8Cvi9fA=="

extern BankUser logged_user;

void logout();

bool trigger_login_process();

struct LoginResponse {
  BankUser user;
  bool     success;
};

struct LoginResponse login(const char* cpf, const char* pwd);

#endif /* CBANK_AUTH_H */
