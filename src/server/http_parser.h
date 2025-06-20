#ifndef CBANK_HTTP_PARSER_H
#define CBANK_HTTP_PARSER_H

#include <stddef.h>
#include <string.h>
#include <strings.h>

#define CRLF "\r\n"

#define MAX_HEADERS 32
#define MAX_KEY_LEN 128
#define MAX_VALUE_LEN 512
#define BODY_LEN 4096

typedef enum {
  HEADER_HOST,
  HEADER_CONTENT_TYPE,
  HEADER_CONTENT_LENGTH,
  HEADER_USER_AGENT,
  HEADER_ACCEPT,
  HEADER_COOKIE,
  HEADER_CONNECTION,
  HEADER_UNKNOWN,
  HEADER_COUNT
} HttpHeaderField;

typedef enum {
  HTTP_OK                    = 200,
  HTTP_NO_CONTENT            = 204,
  HTTP_BAD_REQUEST           = 400,
  HTTP_NOT_FOUND             = 404,
  HTTP_INTERNAL_SERVER_ERROR = 500,
  HTTP_METHOD_NOT_ALLOWED    = 405,
  HTTP_CREATED               = 201,
  HTTP_FORBIDDEN             = 403,
  HTTP_UNAUTHORIZED          = 401
} HttpStatusCode;

typedef enum {
  CONTENT_TYPE_HTML,
  CONTENT_TYPE_JSON,
  CONTENT_TYPE_PLAIN,
  CONTENT_TYPE_CSS,
  CONTENT_TYPE_JAVASCRIPT,
  CONTENT_TYPE_XML,
  CONTENT_TYPE_FORM_URLENCODED,
  CONTENT_TYPE_MULTIPART_FORM_DATA,
  CONTENT_TYPE_OCTET_STREAM,
  CONTENT_TYPE_UNKNOWN
} ContentType;

struct HttpHeader {
  char key[MAX_KEY_LEN];
  char value[MAX_VALUE_LEN];
};

struct HttpRequest {
  char method[8];
  char path[256];
  char version[16];

  struct HttpHeader headers[MAX_HEADERS];
  int               header_count;
  char              body[BODY_LEN];
};

struct HttpResponse {
  const char*       version;
  HttpStatusCode    status_code;
  const char*       status_text;
  struct HttpHeader headers[MAX_HEADERS];
  char*             body;
  int               header_count;
};

int         parse_req_line(const char* raw_req, struct HttpRequest* http_req);
const char* parse_req_headers(const char* header_start, struct HttpRequest* http_req);
void        parse_req_body(const char* body_start, struct HttpRequest* http_req);
void        get_path_template(char* template_content, size_t buf_size, const char* path);
const char* get_header(struct HttpRequest* req, const HttpHeaderField header_field);

#endif /* CBANK_HTTP_PARSER_H */