#include <stddef.h>
#define CRLF "\r\n"

struct HttpRequest {
  char method[8];
  char path[256];
  char version[16];
};

int parse_request_line(const char* req, struct HttpRequest* http_req);
void get_route_html(char* template_content, size_t buf_size, const char* path);