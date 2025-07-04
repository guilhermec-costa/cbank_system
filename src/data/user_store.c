#include "user_store.h"

#include "../orm/select_query.h"
#include "models.h"
#include "store.h"

#include <string.h>

extern Stores stores;

BankUser mount_user_from_line_buf(const char* line_buf) {
  BankUser user;
  memset(&user, 0, sizeof(BankUser));

  char* id_token         = strstr(line_buf, "id=");
  char* name_token       = strstr(line_buf, "name=");
  char* cpf_token        = strstr(line_buf, "cpf=");
  char* pwd_token        = strstr(line_buf, "password=");
  char* is_active_token  = strstr(line_buf, "is_active=");
  char* created_at_token = strstr(line_buf, "created_at=");
  char* updated_at_token = strstr(line_buf, "updated_at=");

  if (!id_token || !name_token || !cpf_token || !pwd_token || !is_active_token ||
      !updated_at_token || !created_at_token) {
    strcpy(user.id, "");
    return user;
  }

  char __id[20];
  char __name[256];
  char __cpf[20];
  char __pwd[256];
  int  __is_active = 0;
  char __created_at[20];
  char __updated_at[20];

  sscanf(id_token, "id=%19[^;];", __id);
  sscanf(name_token, "name=%255[^;];", __name);
  sscanf(cpf_token, "cpf=%19[^;];", __cpf);
  sscanf(pwd_token, "password=%255[^;];", __pwd);
  sscanf(is_active_token, "is_active=%d;", &__is_active);
  sscanf(created_at_token, "created_at=%19[^;];", __created_at);
  sscanf(updated_at_token, "updated_at=%19[^;];", __updated_at);

  strcpy(user.id, __id);
  strcpy(user.name, __name);
  strcpy(user.cpf, __cpf);
  strcpy(user.password, __pwd);
  user.is_active = __is_active;
  strcpy(user.created_at, __created_at);
  strcpy(user.updated_at, __updated_at);

  return user;
};

GetUserByFieldResponse get_user_by_cpf(const char* cpf) {
  return get_user_by_field("cpf", cpf);
};

GetUserByFieldResponse get_user_by_id(const char* id) {
  return get_user_by_field("id", id);
};

GetUserByFieldResponse get_user_by_field(const char* field, const char* value) {
  BankUser     user = {0};
  SelectQuery* q    = new_select_query();
  q->select_all(q, DB_USER_SECTION)->where(q, field, "=", value);
  ResultSet* result = q->execute(q);

  q->destroy(q);

  if (result->rows == 0) {
    result->free(result);
    return (GetUserByFieldResponse){
        .user    = user,
        .success = false,
    };
  };

  char buf[1024] = "";
  result->full_line(result, 0, buf, sizeof(buf));
  user = mount_user_from_line_buf(buf);

  result->free(result);
  return (GetUserByFieldResponse){
      .user    = user,
      .success = true,
  };
}