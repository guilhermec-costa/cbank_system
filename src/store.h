#ifndef CBANK_STORE_H
#define CBANK_STORE_H

#include <stdio.h>

#define DB_USER_SECTION "user.db"

typedef struct _stores {
  struct UserStore {
    const char* store_name;
    FILE* storage;
  } user_store;
} Stores;

FILE* db_instance(const char* f);

void setup_stores();
void terminate_stores();
void get_user_by_id(const char* id);

#endif /* CBANK_STORE_H */
