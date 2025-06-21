#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"
#include "../../data/store.h"
#include "../../core/acc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

static void handle_GET_api_accounts(struct HttpRequest* req, struct HttpResponse* res) {
  int items_count = 0;
  Account* accounts = get_all_accounts(&items_count);
  if(accounts && items_count + 0) {
    for(int i=0; i < items_count; i++) {
      printf("%s", acc_to_line_buf(&accounts[i]));
    }
  }
  free(accounts);
};

static void handle_POST_api_accounts(struct HttpRequest* req, struct HttpResponse* res) {

};

void handle_api_accounts(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  const char* method = req->method;
  if(strcmp(method, "GET") == 0) {
    make_res_first_line(res, HTTP_OK);
    handle_GET_api_accounts(req, res);
  }
  if(strcmp(method, "POST") == 0) {
    make_res_first_line(res, HTTP_NO_CONTENT);
    handle_POST_api_accounts(req, res);
  }

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* json_type = get_content_type_string(CONTENT_TYPE_JSON);
  add_res_header(res, content_type_str, json_type);

  send_http_response(fd, res);
};