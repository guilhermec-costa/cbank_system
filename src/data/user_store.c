#include "../utils.h"
#include "models.h"
#include "store.h"

#include <string.h>

extern Stores stores;

BankUser mount_from_line_buf(const char* line_buf) {
  BankUser user;
  RESET_ENTITY(user);

  char* id_token         = strstr(line_buf, "id=");
  char* pwd_token        = strstr(line_buf, "password=");
  char* email_token      = strstr(line_buf, "email=");
  char* cpf_token        = strstr(line_buf, "cpf=");
  char* name_token       = strstr(line_buf, "name=");
  char* created_at_token = strstr(line_buf, "created_at=");
  char* updated_at_token = strstr(line_buf, "updated_at=");
  char* is_active_token  = strstr(line_buf, "is_active=");

  if (!id_token || !pwd_token || !email_token || !name_token || !cpf_token || !created_at_token ||
      !updated_at_token || !is_active_token) {
    strcpy(user.id, "");
    return user;
  }

  char __id[20];
  char __pwd[PWD_MAX_CHAR_CONSTRAINT];
  char __email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
  char __name[50];
  char __cpf[CPF_DIGITS];
  char __created_at[sizeof(VALID_DATETIME_STR)];
  char __updated_at[sizeof(VALID_DATETIME_STR)];
  char __is_active[3];

  sscanf(id_token, "id=%19[^;];", __id);
  sscanf(pwd_token, "password=%34[^;];", __pwd);
  sscanf(email_token, "email=%49[^;];", __email);
  sscanf(name_token, "name=%49[^;];", __name);
  sscanf(cpf_token, "cpf=%10[^;];", __cpf);
  sscanf(created_at_token, "created_at=%19[^;];", __created_at);
  sscanf(updated_at_token, "updated_at=%19[^;];", __updated_at);
  sscanf(is_active_token, "is_active=%2[^;];", __is_active);

  strcpy(user.id, __id);
  strcpy(user.email, __email);
  strcpy(user.name, __name);
  strcpy(user.cpf, __cpf);
  strcpy(user.password, __pwd);
  user.is_active = *__is_active == 1 ? true : false;
  strcpy(user.created_at, __created_at);
  strcpy(user.updated_at, __updated_at);

  return user;
}

BankUser get_user_by_cpf(const char* cpf) {
  FILE*    user_store = get_storage_for_reading(DB_USER_SECTION);
  char     line_buf[256];
  BankUser user;
  RESET_ENTITY(user);

  char* cpf_token = NULL;
  while (fgets(line_buf, sizeof(line_buf), user_store)) {
    terminate_str_by_newline(line_buf);
    cpf_token = strstr(line_buf, "cpf=");
    if (!cpf_token)
      continue;

    char __cpf[CPF_DIGITS];
    sscanf(cpf_token, "cpf=%10[^;];", __cpf);

    if (strcmp(__cpf, cpf) == 0) {
      user = mount_from_line_buf(line_buf);
      break;
    }
  }
  return user;
}
