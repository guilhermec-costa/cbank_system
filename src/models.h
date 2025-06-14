#ifndef CBANK_MODELS_H
#define CBANK_MODELS_H

#define REGISTRATION_ID_MAX_CHAR_CONSTRAINT 35
#define PWD_MAX_CHAR_CONSTRAINT 35
#define VALID_DATETIME_STR "2004-06-04-21-50-32"

#include <stdbool.h>

typedef struct auth_credentails {
  char id[REGISTRATION_ID_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} AuthCredentials;

typedef struct bank_user {
  char id[REGISTRATION_ID_MAX_CHAR_CONSTRAINT];
  char name[50];
  char password[PWD_MAX_CHAR_CONSTRAINT];
  bool is_active;
  char created_at[sizeof(VALID_DATETIME_STR)];
  char updated_at[sizeof(VALID_DATETIME_STR)];
} BankUser;

#endif /* CBANK_MODELS_H */
