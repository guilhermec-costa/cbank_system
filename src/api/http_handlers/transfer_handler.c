#include "handlers.h"

// void handle_transfer(int fd, struct HttpRequest* req, struct HttpResponse* res) {
//   const char* template = load_template_to_string(INDEX_TEMPLATE_PATH);
//   add_body(res, template);
//   add_content_type(res, CONTENT_TYPE_HTML);
//   add_content_len(res, strlen(res->body));
//   make_res_first_line(res, HTTP_OK);
//   send_http_response(fd, res);
// }