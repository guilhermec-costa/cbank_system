#include "handlers.h"

#include "http_parser.h"
#include "http_utils.h"
#include "route_contants.h"
#include "router.h"
#include "templates_constants.h"

#include <stdio.h>

void handle_home(struct HttpRequest* req, struct HttpResponse* res) {
  get_path_template(res->body, sizeof(res->body), INDEX_ROUTE_PATH);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(CONTENT_TYPE_HTML);
  add_res_header(res, content_type_str, html_type);

  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", res->body ? strlen(res->body) : 0);
  add_res_header(res, content_length_str, body_res_buf);
}

void handle_accounts(struct HttpRequest* req, struct HttpResponse* res) {
  get_path_template(res->body, sizeof(res->body), ACCOUNTS_ROUTE_PATH);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(CONTENT_TYPE_HTML);
  add_res_header(res, content_type_str, html_type);

  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", res->body ? strlen(res->body) : 0);
  add_res_header(res, content_length_str, body_res_buf);
}

void handle_login(struct HttpRequest* req, struct HttpResponse* res) {
  get_path_template(res->body, sizeof(res->body), LOGIN_ROUTE_PATH);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(CONTENT_TYPE_HTML);
  add_res_header(res, content_type_str, html_type);

  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", res->body ? strlen(res->body) : 0);
  add_res_header(res, content_length_str, body_res_buf);
};