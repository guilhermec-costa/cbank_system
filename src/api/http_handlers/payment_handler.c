#include "../../render/engine.h"
#include "../../server/http_utils.h"
#include "../../server/router.h"
#include "../../server/templates_constants.h"
#include "handlers.h"

void handle_payments(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  const char* template = load_template_to_string(PAYMENTS_TEMPLATE_PATH);
  add_body(res, template);
  add_content_type(res, CONTENT_TYPE_HTML);
  add_content_len(res, strlen(res->body));
  make_res_first_line(res, HTTP_OK);
  send_http_response(fd, res);
}