#ifndef CBANK_STORE_H
#define CBANK_STORE_H

#include <stdio.h>

#include "models.h"

#define DB_USER_SECTION "user.db"
#define DB_ID_TRACKER_SECTION "id_tracker.db"

typedef struct _stores {
  struct UserStore {
    const char* store_name;
    FILE* storage;
  } user_store;

  struct IdTrackerStore {
    const char* store_name;
    FILE* storage;
  } id_tracker_store;
} Stores;

FILE* db_instance(const char* f);
void setup_stores();
void terminate_stores();

BankUser get_user_by_acc_id(const char* account_id);
bool email_already_registered(const char* email);

void reset_entity_attr_mem(void* ent, size_t ent_s);
void updt_next_identity(const char* store_name);
void mov_store_cursor(const char* store_name, int pos);
int get_next_identity(const char* store_name);

typedef struct CreateUserDTO {
  char name[50];
  char email[50];
  char password[PWD_MAX_CHAR_CONSTRAINT];
} CreateUserDTO;

bool id_tracker_has_store(const char* store_name);
FILE* get_storage(const char* store_name);

#define RESET_ENTITY(entity) reset_entity_attr_mem(&(entity), sizeof(entity))

#define NEXT_USER_IDENTITY get_next_identity(DB_USER_SECTION);

#endif /* CBANK_STORE_H */
