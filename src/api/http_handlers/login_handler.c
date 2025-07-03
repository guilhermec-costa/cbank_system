#include "../../core/auth.h"
#include "../../jwt/jwt.h"
#include "../../render/engine.h"
#include "../../secret/manager.h"
#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"
#include "../../server/templates_constants.h"
#include "../schemas/login_validation_schema.h"

void handle_GET_login(struct HttpRequest* req, struct HttpResponse* res) {
  const char* template = load_template_to_string(LOGIN_TEMPLATE_PATH);
  add_body(res, render_template(template, NULL, 0));
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

  if (!parser(req, &schema)) {
    add_content_type(res, CONTENT_TYPE_PLAIN);
    make_res_first_line(res, HTTP_BAD_REQUEST);
    strcpy(res->body, "Malformed JSON");
    add_body(res, "Malformed JSON");
    return false;
  };

  if (!validate_login_schema(&schema)) {
    add_content_type(res, CONTENT_TYPE_PLAIN);
    make_res_first_line(res, HTTP_BAD_REQUEST);
    add_body(res, "Invalid fields");
    return false;
  };

  const struct LoginResponse logged = login(schema.cpf, schema.password);
  if (!logged.success) {
    make_res_first_line(res, HTTP_UNAUTHORIZED);
    add_body(res, "Invalid credentials");
    return false;
  }

  char* token = create_jwt_for_user(logged.user.id, logged.user.name);
  if (!token) {
    make_res_first_line(res, HTTP_INTERNAL_SERVER_ERROR);
    add_body(res, "Failed to generate token");
    return false;
  };

  make_res_first_line(res, HTTP_OK);
  add_content_type(res, CONTENT_TYPE_JSON);
  add_res_token_cookie(res, token);
  free(token);
  return true;
};

void handle_login(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  if (strcmp(req->method, "GET") == 0) {
    const char* token = get_req_cookie(req, "token");
    if (token) {
      const char* payload = jwt_validate(token, get_env("JWT_SECRET"));
      if (payload) {
        redirect(fd, res, ACCOUNTS_ROUTE_PATH);
        return;
      }
    }
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