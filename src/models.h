#ifndef CBANK_MODELS_H
#define CBANK_MODELS_H

#define REGISTRATION_NAME_MAX_CHAR_CONSTRAINT 50
#define REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT 50
#define PWD_MAX_CHAR_CONSTRAINT 35
#define ACC_ID_MAX_CHAR_CONSTRAINT 10
#define CPF_DIGITS 11
#define VALID_DATETIME_STR "2004-06-04-21-50-32"

#include <stdbool.h>

typedef char _created_at[sizeof(VALID_DATETIME_STR)];
typedef char _updated_at[sizeof(VALID_DATETIME_STR)];

#define ENTITY_ID _entity_id id;
#define CREATED_AT _created_at created_at;
#define UPDATED_AT _updated_at updated_at;

typedef struct auth_credentails {
  char account_id[REGISTRATION_NAME_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} AuthCredentials;

typedef struct bank_user {
  char id[20];
  char acc_id[ACC_ID_MAX_CHAR_CONSTRAINT];
  char name[REGISTRATION_NAME_MAX_CHAR_CONSTRAINT];
  char cpf[CPF_DIGITS];
  char email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
  bool is_active;
  CREATED_AT;
  UPDATED_AT;
} BankUser;

typedef struct account {
  char   id[20];
  char   user_id_fk[20];
  double balance;
  CREATED_AT;
  UPDATED_AT;
} Account;

typedef struct transaction {
  char id[20];
  CREATED_AT;
  UPDATED_AT;
} Transaction;

#endif /* CBANK_MODELS_H */
