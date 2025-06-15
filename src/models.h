#ifndef CBANK_MODELS_H
#define CBANK_MODELS_H

#define REGISTRATION_NAME_MAX_CHAR_CONSTRAINT 50
#define REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT 50
#define PWD_MAX_CHAR_CONSTRAINT 35
#define ACC_ID_MAX_CHAR_CONSTRAINT 10
#define VALID_DATETIME_STR "2004-06-04-21-50-32"

#include <stdbool.h>

typedef struct auth_credentails {
  char account_id[REGISTRATION_NAME_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} AuthCredentials;

typedef struct bank_user {
  char id[20];
  char acc_id[ACC_ID_MAX_CHAR_CONSTRAINT];
  char name[REGISTRATION_NAME_MAX_CHAR_CONSTRAINT];
  char email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
  bool is_active;
  char created_at[sizeof(VALID_DATETIME_STR)];
  char updated_at[sizeof(VALID_DATETIME_STR)];
} BankUser;

typedef struct account {
  char   id[20];
  char   user_id_fk[20];
  double balance;
  char   created_at[sizeof(VALID_DATETIME_STR)];
  char   updated_at[sizeof(VALID_DATETIME_STR)];
} Account;

#endif /* CBANK_MODELS_H */
