#ifndef CBANK_AUTH_H
#define CBANK_AUTH_H

#include "../data/models.h"
#include "../data/store.h"

#include <stdbool.h>

#define MAX_LOGIN_TRIES 3
#define NON_EXISTING_USER_ID_FLAG ""
#define PWD_HASH_SALT "PHToXCpRR1qGC2G8Cvi9fA=="

extern BankUser logged_user;

bool            try_login(AuthCredentials cred);
CreateUserDTO   register_user_form();
void            create_user(CreateUserDTO payload);
AuthCredentials make_in_mem_creds(const char* id, const char* pwd);
void            logout();

bool trigger_login_process();

struct LoginResponse {
  BankUser user;
  bool     success;
};

struct LoginResponse login(const char* cpf, const char* pwd);

#endif /* CBANK_AUTH_H */
