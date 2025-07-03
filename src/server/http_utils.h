#include "http_parser.h"

const char* get_content_type_string(ContentType type);
const char* get_status_text(HttpStatusCode code);
const char* get_header_field_name(const HttpHeaderField field);
char*       get_header(struct HttpRequest* req, const HttpHeaderField header_field);
char*       get_res_header(struct HttpResponse* res, HttpHeaderField header_field);
char*       get_req_cookie(struct HttpRequest* req, const char* cookie_name);
void        make_non_authorized(struct HttpResponse* res);