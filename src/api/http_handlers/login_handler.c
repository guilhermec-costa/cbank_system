#include "../../core/auth.h"
#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"
#include "../schemas/login_validation_schema.h"

#include <stdio.h>

void handle_GET_login(struct HttpRequest* req, struct HttpResponse* res) {
  get_path_template(res->body, sizeof(res->body), LOGIN_ROUTE_PATH);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(CONTENT_TYPE_HTML);
  add_res_header(res, content_type_str, html_type);

  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", strlen(res->body));
  add_res_header(res, content_length_str, body_res_buf);
  make_res_first_line(res, HTTP_OK);
};

bool handle_POST_login(struct HttpRequest* req, struct HttpResponse* res) {
  LoginSchema     schema       = {0};
  const char*     content_type = get_header(req, HEADER_CONTENT_TYPE);
  LoginBodyParser parser       = strcmp(content_type, "application/json") == 0
                                     ? parse_login_json_schema
                                     : parse_login_xwf_urlencoded_schema;

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* plain_type       = get_content_type_string(CONTENT_TYPE_PLAIN);
  add_res_header(res, content_type_str, plain_type);

  if (!parser(req->body, &schema)) {
    make_res_first_line(res, HTTP_BAD_REQUEST);
    strcpy(res->body, "Malformed JSON");
    return false;
  };

  if (!validate_login_schema(&schema)) {
    make_res_first_line(res, HTTP_BAD_REQUEST);
    strcpy(res->body, "Invalid fields");
    return false;
  };

  AuthCredentials auth = {0};
  strcpy(auth.cpf, schema.cpf);
  strcpy(auth.password, schema.password);

  if (!try_login(auth)) {
    make_res_first_line(res, HTTP_UNAUTHORIZED);
    strcpy(res->body, "Invalid credentials");
    return false;
  };

  make_res_first_line(res, HTTP_NO_CONTENT);
  return true;
};

void handle_login(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  if (strcmp(req->method, "GET") == 0) {
    handle_GET_login(req, res);
    send_http_response(fd, res);
    return;
  }
  if (strcmp(req->method, "POST") == 0) {
    bool success = handle_POST_login(req, res);
    if (!success) {
      send_http_response(fd, res);
      return;
    }
    redirect(fd, res, ACCOUNTS_ROUTE_PATH);
  }
};