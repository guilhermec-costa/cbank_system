#include <string.h>

#include "store.h"

extern Stores stores;

BankUser get_user_by_acc_id(const char* acc_id) {
  FILE* user_store = get_storage(DB_USER_SECTION);
  char line_buf[256];
  BankUser user;
  RESET_ENTITY(user);

  char* id_token = NULL;
  char* pwd_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), user_store)) {
    id_token = strstr(line_buf, "acc_id=");
    pwd_token = strstr(line_buf, "pwd=");

    if (!id_token || !pwd_token) {
      continue;
    }

    char _tmp_id[ACC_ID_MAX_CHAR_CONSTRAINT];
    sscanf(id_token, "acc_id=%9[^;];", _tmp_id);

    if (strcmp(_tmp_id, acc_id) == 0) {
      strcpy(user.id, _tmp_id);

      char _tmp_pwd[PWD_MAX_CHAR_CONSTRAINT];
      sscanf(pwd_token, "pwd=%34[^;];", _tmp_pwd);
      strcpy(user.password, _tmp_pwd);

      return user;
    }
  }

  strcpy(user.id, "");
  return user;
}

bool email_already_registered(const char* email) {
  FILE* user_store = get_storage(DB_USER_SECTION);
  char line_buf[256];

  char* email_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), user_store)) {
    email_token = strstr(line_buf, "email=");
    if (!email_token) continue;

    char _tmp_email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
    sscanf(email_token, "email=%49[^;];", _tmp_email);

    if (strcmp(_tmp_email, email) == 0) {
      return true;
    }
  }
  return false;
}
