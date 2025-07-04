#include "../../core/acc.h"
#include "../../core/json_builder.h"
#include "../../data/store.h"
#include "../../data/user_store.h"
#include "../../orm/select_query.h"
#include "../../render/engine.h"
#include "../../server/http_utils.h"
#include "../../server/router.h"
#include "../../server/templates_constants.h"
#include "../dtos/acc_models.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GetAccountDTO account_to_get_account_DTO(Account* acc) {
  GetAccountDTO accountDTO;
  strcpy(accountDTO.id, acc->id);
  accountDTO.balance = acc->balance;
  strcpy(accountDTO.created_at, acc->created_at);
  strcpy(accountDTO.updated_at, acc->updated_at);
  return accountDTO;
};

void handle_accounts(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  ListAccountsResponse accounts    = list_accounts();
  GetAccountDTO*       accountsDTO = calloc(accounts.item_count, sizeof(GetAccountDTO));

  if (accounts.accounts && accounts.item_count > 0) {
    for (int i = 0; i < accounts.item_count; i++) {
      accountsDTO[i] = account_to_get_account_DTO(&accounts.accounts[i]);
    }
  }
  free(accounts.accounts);
  const char*            user_id   = req->authenticated_jwt.sub;
  GetUserByFieldResponse user      = get_user_by_id(user_id);
  const char*            template  = load_template_to_string(ACCOUNTS_TEMPLATE_PATH);
  TemplateVar            context[] = {{"username", user.user.name}, {"total_balance", "20"}};
  add_body(res, render_template(template, context, 2));
  make_res_first_line(res, HTTP_OK);
  add_content_type(res, CONTENT_TYPE_HTML);
  add_content_len(res, strlen(res->body));
  send_http_response(fd, res);
};

const char* accountDTO_to_json_obj(JsonBuilder* jb, GetAccountDTO* acc_dto) {
  json_add_comma_if_needed(jb);
  json_start_object(jb);
  json_add_string(jb, "id", acc_dto->id);
  json_add_string(jb, "created_at", acc_dto->created_at);
  json_add_float(jb, "balance", acc_dto->balance);
  json_end_object(jb);
  return json_build(jb);
};

static void handle_GET_api_accounts(struct HttpRequest* req, struct HttpResponse* res) {
  ListAccountsResponse accounts    = list_accounts();
  GetAccountDTO*       accountsDTO = calloc(accounts.item_count, sizeof(GetAccountDTO));

  if (accounts.accounts && accounts.item_count + 0) {
    for (int i = 0; i < accounts.item_count; i++) {
      accountsDTO[i] = account_to_get_account_DTO(&accounts.accounts[i]);
    }
  }
  free(accounts.accounts);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* json_type        = get_content_type_string(CONTENT_TYPE_JSON);
  add_res_header(res, content_type_str, json_type);

  JsonBuilder jb;
  json_init(&jb);

  json_start_object(&jb);
  json_start_array(&jb, "data");

  for (int i = 0; i < accounts.item_count; i++) {
    accountDTO_to_json_obj(&jb, &accountsDTO[i]);
  }

  free(accountsDTO);
  json_end_array(&jb);
  json_end_object(&jb);

  snprintf(res->body, sizeof(res->body), "%s", json_build(&jb));
};

static void handle_POST_api_accounts(struct HttpRequest* req, struct HttpResponse* res) {

};

void handle_api_accounts(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  const char* method = req->method;
  if (strcmp(method, "GET") == 0) {
    make_res_first_line(res, HTTP_OK);
    handle_GET_api_accounts(req, res);
  }
  if (strcmp(method, "POST") == 0) {
    make_res_first_line(res, HTTP_NO_CONTENT);
    handle_POST_api_accounts(req, res);
  }

  send_http_response(fd, res);
};