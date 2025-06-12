#ifndef CBANK_AUTH_H
#define CBANK_AUTH_H

#include <stdbool.h>
#include "models.h"

#define MAX_LOGIN_TRIES 3
#define NON_EXISTING_USER_ID_FLAG ""
#define PWD_HASH_SALT "PHToXCpRR1qGC2G8Cvi9fA=="

bool try_login(AuthCredentials cred);
void create_user(AuthCredentials cred);
AuthCredentials make_in_mem_user(const char* id, const char* pwd);
void logout();

#endif /* CBANK_AUTH_H */
