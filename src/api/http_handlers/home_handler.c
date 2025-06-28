#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"

void handle_home(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  get_route_render_template(res->body, sizeof(res->body), INDEX_ROUTE_PATH);
  add_content_type(res, CONTENT_TYPE_HTML);
  add_content_len(res, strlen(res->body));
  make_res_first_line(res, HTTP_OK);
  send_http_response(fd, res);
}
