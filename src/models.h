#ifndef CBANK_MODELS_H
#define CBANK_MODELS_H

#define NAME_MAX_CHAR_CONSTRAINT 35
#define PWD_MAX_CHAR_CONSTRAINT 35

typedef struct auth_credentails {
  char id[NAME_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} AuthCredentials;

#endif /* CBANK_MODELS_H */
