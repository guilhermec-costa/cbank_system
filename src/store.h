#ifndef CBANK_STORE_H
#define CBANK_STORE_H

#include <stdio.h>

#define DB_USER_SECTION "user.db"

typedef struct _stores {
  FILE* user_store;
} Stores;

FILE* db_instance(const char* f);

void setup_stores();

#endif /* CBANK_STORE_H */
