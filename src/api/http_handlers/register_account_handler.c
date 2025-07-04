#include "../../core/user.h"
#include "../../render/engine.h"
#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"
#include "../../server/templates_constants.h"
#include "../dtos/user_models.h"
#include "../schemas/register_account_validation_schema.h"
#include "handlers.h"

static void handle_GET_register_account(struct HttpRequest* req, struct HttpResponse* res) {
  const char* template = load_template_to_string(REGISTER_ACCOUNT_TEMPLATE_PATH);
  add_body(res, render_template(template, NULL, 0));
  add_content_type(res, CONTENT_TYPE_HTML);
  add_content_len(res, strlen(res->body));
  make_res_first_line(res, HTTP_OK);
};

static bool handle_POST_register_account(struct HttpRequest* req, struct HttpResponse* res) {
  RegisterAccountSchema     schema = {0};
  RegisterAccountBodyParser parser =
      strcmp(get_header(req, HEADER_CONTENT_TYPE), "application/json") == 0
          ? parse_register_acc_json_schema
          : parse_register_acc_xwf_urlencoded_schema;

  if (!parser(req, &schema)) {
    add_content_type(res, CONTENT_TYPE_PLAIN);
    make_res_first_line(res, HTTP_BAD_REQUEST);
    add_body(res, "Malformed JSON request body");
    return false;
  };

  CreateUserDTO       user_data = user_dto_from_register_acc_schema(&schema);
  MakeNewUserResponse result    = make_new_user(user_data);
  if (!result.success) {
    make_res_first_line(res, HTTP_BAD_REQUEST);
    add_body(res, result.message);
    return false;
  }

  make_res_first_line(res, HTTP_NO_CONTENT);
  return true;
};

void handle_register_acocunt(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  if (strcmp(req->method, "GET") == 0) {
    handle_GET_register_account(req, res);
    send_http_response(fd, res);
    return;
  }
  if (strcmp(req->method, "POST") == 0) {
    bool success = handle_POST_register_account(req, res);
    if (!success) {
      send_http_response(fd, res);
      return;
    }
    redirect(fd, res, ACCOUNTS_ROUTE_PATH);
  }
}