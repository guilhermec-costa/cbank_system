#include "auth.h"

#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "colorization.h"
#include "store.h"
#include "utils.h"

extern Stores stores;

bool try_login(AuthCredentials c) {
  BankUser user = get_user_by_account_id(c.account_id);
  if (strcmp(user.id, NON_EXISTING_USER_ID_FLAG) == 0) return false;

  const char* ck_pwd_hash = hash_str(c.password, PWD_HASH_SALT);
  if (strcmp(ck_pwd_hash, user.password) != 0) return false;

  return true;
}

AuthCredentials make_in_mem_creds(const char* acc_id, const char* pwd) {
  AuthCredentials c;
  strcpy(c.account_id, acc_id);
  strcpy(c.password, pwd);
  return c;
}

void create_user(CreateUserDTO payload) {
  const bool existing_email = email_already_registered(payload.email);
  if (existing_email) {
    printf("Could not create user. Email already exists");
    return;
  }

  const char* const pwd_hash = hash_str(payload.password, PWD_HASH_SALT);

  const char* acc_id = gen_acc_id();
  const int next_user_id = NEXT_USER_IDENTITY;

  const struct tm at_now = now();
  char date_buf[100];
  const char* formatted_now = fmt_date(date_buf, sizeof(date_buf), at_now);

  mov_store_cursor(stores.user_store.store_name, SEEK_END);
  fprintf(stores.user_store.storage,
          "id=%d;acc_id=%s;email=%s;pwd=%s;created_at=%s;updated_at=%s\n",
          next_user_id, acc_id, payload.email, pwd_hash, formatted_now,
          "NULL");

  updt_next_identity(stores.user_store.store_name);

  fflush(stores.user_store.storage);
  printf("Your account has been created!");
};

CreateUserDTO register_user_form() {
  printf("\n%s==============================\n", COLOR_CYAN);
  printf("        User Registration\n");
  printf("==============================%s\n\n", COLOR_RESET);

  char new_email[REGISTRATION_EMAIL_MAX_CHAR_CONSTRAINT];
  char new_name[REGISTRATION_NAME_MAX_CHAR_CONSTRAINT];
  char new_pwd[PWD_MAX_CHAR_CONSTRAINT];

  printf("%sPlease fill in the following details:%s\n\n", COLOR_GREEN,
         COLOR_RESET);

  ask_null_terminated_input_str(new_email, sizeof(new_email),
                                COLOR_YELLOW "📧  Email: " COLOR_RESET);
  ask_null_terminated_input_str(new_name, sizeof(new_name),
                                COLOR_YELLOW "👤  Name: " COLOR_RESET);
  ask_null_terminated_input_str(new_pwd, sizeof(new_pwd),
                                COLOR_YELLOW "🔑  Password: " COLOR_RESET);
  CreateUserDTO user = {};
  RESET_ENTITY(user);
  strcpy(user.email, new_email);
  strcpy(user.password, new_pwd);
  strcpy(user.name, new_name);

  printf("\n%s✅ User registration form completed!%s\n", COLOR_GREEN,
         COLOR_RESET);
  printf("%s--------------------------------------%s\n", COLOR_CYAN,
         COLOR_RESET);
  return user;
}

bool trigger_login_process() {
  char tmp_id[ACC_ID_MAX_CHAR_CONSTRAINT];
  char tmp_pwd[PWD_MAX_CHAR_CONSTRAINT];
  bool logged = false;
  int login_tries = 0;

  printf("\n%s==============================\n", COLOR_CYAN);
  printf("           User Login\n");
  printf("==============================%s\n\n", COLOR_RESET);

  while (!logged && login_tries < MAX_LOGIN_TRIES) {
    printf("%s🔐 Attempt %d of %d%s\n", COLOR_MAGENTA, login_tries + 1,
           MAX_LOGIN_TRIES, COLOR_RESET);
    printf("%s------------------------------%s\n", COLOR_CYAN, COLOR_RESET);

    ask_null_terminated_input_str(
        tmp_id, sizeof(tmp_id), COLOR_YELLOW "🆔  Account ID: " COLOR_RESET);
    ask_null_terminated_input_str(tmp_pwd, sizeof(tmp_pwd),
                                  COLOR_YELLOW "🔑  Password: " COLOR_RESET);

    const AuthCredentials user = make_in_mem_creds(tmp_id, tmp_pwd);
    logged = try_login(user);
    if (!logged) {
      login_tries++;
      printf("\n%s❌ Failed to login. Please try again.%s\n\n", COLOR_RED,
             COLOR_RESET);
      continue;
    }

    printf("\n%s✅ Login successful! Welcome!%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s--------------------------------------%s\n\n", COLOR_CYAN,
           COLOR_RESET);
    return true;
  }

  return false;
}
