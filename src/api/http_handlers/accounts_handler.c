#include "../../core/acc.h"
#include "../../core/json_builder.h"
#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"
#include "../dtos/acc_models.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_accounts(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  get_path_template(res->body, sizeof(res->body), ACCOUNTS_ROUTE_PATH);

  make_res_first_line(res, HTTP_OK);
  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(CONTENT_TYPE_HTML);
  add_res_header(res, content_type_str, html_type);

  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", strlen(res->body));
  add_res_header(res, content_length_str, body_res_buf);

  send_http_response(fd, res);
};

GetAccountDTO account_to_get_account_DTO(Account* acc) {
  GetAccountDTO accountDTO;
  strcpy(accountDTO.id, acc->id);
  accountDTO.balance = acc->balance;
  strcpy(accountDTO.created_at, acc->created_at);
  strcpy(accountDTO.updated_at, acc->updated_at);
  return accountDTO;
};

const char* accountDTO_to_json_obj(JsonBuilder* jb, GetAccountDTO* acc_dto) {
  json_init(jb);
  json_start_object(jb);
  json_add_string(jb, "id", acc_dto->id);
  json_add_string(jb, "created_at", acc_dto->created_at);
  json_add_float(jb, "balance", acc_dto->balance);
  json_end_object(jb);
  return json_build(jb);
};

static void handle_GET_api_accounts(struct HttpRequest* req, struct HttpResponse* res) {
  int            items_count = 0;
  Account*       accounts    = get_all_accounts(&items_count);
  GetAccountDTO* accountsDTO = calloc(items_count, sizeof(GetAccountDTO));

  if (accounts && items_count + 0) {
    for (int i = 0; i < items_count; i++) {
      accountsDTO[i] = account_to_get_account_DTO(&accounts[i]);
    }
  }
  free(accounts);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* json_type        = get_content_type_string(CONTENT_TYPE_JSON);
  add_res_header(res, content_type_str, json_type);

  JsonBuilder jb;
  const char* json = accountDTO_to_json_obj(&jb, accountsDTO);
  snprintf(res->body, sizeof(res->body), "%s", json);
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