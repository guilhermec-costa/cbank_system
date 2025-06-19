#define CRLF "\r\n"

struct HttpRequest {
  char method[8];
  char path[256];
  char version[16];
};

int parse_request_line(const char* req, struct HttpRequest* http_req);