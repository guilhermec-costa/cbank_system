#include "../../server/http_utils.h"
#include "../../server/route_contants.h"
#include "../../server/router.h"

#include <stdio.h>

void handle_home(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  get_path_template(res->body, sizeof(res->body), INDEX_ROUTE_PATH);

  const char* content_type_str = get_header_field_name(HEADER_CONTENT_TYPE);
  const char* html_type        = get_content_type_string(CONTENT_TYPE_HTML);
  add_res_header(res, content_type_str, html_type);

  char        body_res_buf[32];
  const char* content_length_str = get_header_field_name(HEADER_CONTENT_LENGTH);
  snprintf(body_res_buf, sizeof(body_res_buf), "%zu", strlen(res->body));
  add_res_header(res, content_length_str, body_res_buf);

  make_res_first_line(res, HTTP_OK);
  send_http_response(fd, res);
}
