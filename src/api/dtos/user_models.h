#ifndef CBANK_USER_MODELS_H
#define CBANK_USER_MODELS_H

#include "../../data/models.h"

typedef struct CreateUserDTO {
  char name[50];
  char email[50];
  char cpf[CPF_DIGITS];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} CreateUserDTO;
#endif /* CBANK_USER_MODELS_H */