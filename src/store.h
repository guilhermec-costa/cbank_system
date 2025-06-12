#ifndef CBANK_STORE_H
#define CBANK_STORE_H

#include <stdio.h>
#include "models.h"

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
BankUser get_user_by_id(const char* id);
void reset_entity_attr_mem(void* ent, size_t ent_s);

#define RESET_ENTITY(entity) reset_entity_attr_mem(&(entity), sizeof(entity))

#endif /* CBANK_STORE_H */
