#include "store.h"

#include <string.h>

extern Stores stores;

BankUser mount_from_line_buf(const char* line_buf) {
  BankUser user;
  RESET_ENTITY(user);
  char* id_token         = NULL;
  char* acc_id_token     = NULL;
  char* pwd_token        = NULL;
  char* email_token      = NULL;
  char* created_at_token = NULL;
  char* updated_at_token = NULL;
  char* is_active_token  = NULL;

  id_token         = strstr(line_buf, "id=");
  acc_id_token     = strstr(line_buf, "acc_id=");
  pwd_token        = strstr(line_buf, "pwd=");
  email_token      = strstr(line_buf, "email=");
  created_at_token = strstr(line_buf, "created_at=");
  updated_at_token = strstr(line_buf, "updated_at=");
  is_active_token  = strstr(line_buf, "is_active=");

  if (!id_token || !acc_id_token || !pwd_token || !email_token || !created_at_token ||
      !updated_at_token || !is_active_token) {
    strcpy(user.id, "");
    return user;
  }

  char __id[20];
  char __acc_id[ACC_ID_MAX_CHAR_CONSTRAINT];
  char __pwd[PWD_MAX_CHAR_CONSTRAINT];
  char __email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
  char __created_at[sizeof(VALID_DATETIME_STR)];
  char __updated_at[sizeof(VALID_DATETIME_STR)];
  char __is_active[3];

  sscanf(id_token, "id=%19[^;];", __id);
  sscanf(acc_id_token, "acc_id=%9[^;];", __acc_id);
  sscanf(pwd_token, "pwd=%34[^;];", __pwd);
  sscanf(email_token, "email=%49[^;];", __email);
  sscanf(created_at_token, "created_at=%19[^;];", __created_at);
  sscanf(updated_at_token, "updated_at=%19[^;];", __updated_at);
  sscanf(is_active_token, "is_active=%3[^;];", __is_active);

  strcpy(user.id, __id);
  strcpy(user.acc_id, __acc_id);
  strcpy(user.email, __email);
  strcpy(user.password, __pwd);
  user.is_active = *__is_active == 1 ? true : false;
  strcpy(user.created_at, __created_at);
  strcpy(user.updated_at, __updated_at);

  return user;
}

BankUser get_user_by_acc_id(const char* acc_id) {
  FILE*    user_store = get_storage(DB_USER_SECTION);
  char     line_buf[256];
  BankUser user;
  RESET_ENTITY(user);

  char* acc_id_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), user_store)) {
    acc_id_token = strstr(line_buf, "acc_id=");
    if(!acc_id_token) continue;

    char __acc_id[ACC_ID_MAX_CHAR_CONSTRAINT];
    sscanf(acc_id_token, "acc_id=%9[^;];", __acc_id);

    if (strcmp(__acc_id, acc_id) == 0) {
      user = mount_from_line_buf(line_buf);
      break;
    }
  }
  return user;
}

bool email_already_registered(const char* email) {
  FILE* user_store = get_storage(DB_USER_SECTION);
  char  line_buf[256];

  char* email_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), user_store)) {
    email_token = strstr(line_buf, "email=");
    if (!email_token)
      continue;

    char _tmp_email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
    sscanf(email_token, "email=%49[^;];", _tmp_email);

    if (strcmp(_tmp_email, email) == 0) {
      return true;
    }
  }
  return false;
}
