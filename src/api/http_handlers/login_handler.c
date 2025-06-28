#include "../../core/auth.h"
#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"
#include "../schemas/login_validation_schema.h"

void handle_GET_login(struct HttpRequest* req, struct HttpResponse* res) {
  get_route_render_template(res->body, sizeof(res->body), LOGIN_ROUTE_PATH);
  add_content_type(res, CONTENT_TYPE_HTML);
  add_content_len(res, strlen(res->body));
  make_res_first_line(res, HTTP_OK);
};

bool handle_POST_login(struct HttpRequest* req, struct HttpResponse* res) {
  LoginSchema     schema       = {0};
  const char*     content_type = get_header(req, HEADER_CONTENT_TYPE);
  LoginBodyParser parser       = strcmp(content_type, "application/json") == 0
                                     ? parse_login_json_schema
                                     : parse_login_xwf_urlencoded_schema;
  add_content_type(res, CONTENT_TYPE_PLAIN);

  if (!parser(req, &schema)) {
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