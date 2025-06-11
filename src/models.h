#pragma once

#define NAME_MAX_CHAR_CONSTRAINT 35
#define PWD_MAX_CHAR_CONSTRAINT 35

struct AuthCredentials {
  char name[NAME_MAX_CHAR_CONSTRAINT];
  char password[PWD_MAX_CHAR_CONSTRAINT];
};

