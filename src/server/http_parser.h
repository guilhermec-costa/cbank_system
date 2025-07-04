#ifndef CBANK_HTTP_PARSER_H
#define CBANK_HTTP_PARSER_H

#define _POSIX_C_SOURCE 200809L

#include "../jwt/jwt.h"

#include <stddef.h>
#include <string.h>
#include <strings.h>

#define CRLF "\r\n"

#define MAX_HEADERS 32
#define MAX_KEY_LEN 128
#define MAX_VALUE_LEN 1024
#define BODY_LEN 18000
#define MAX_COOKIES 1000

typedef enum {
  HEADER_HOST,
  HEADER_CONTENT_TYPE,
  HEADER_CONTENT_LENGTH,
  HEADER_USER_AGENT,
  HEADER_ACCEPT,
  HEADER_COOKIE,
  HEADER_CONNECTION,
  HEADER_UNKNOWN,
  HEADER_AUTHORIZATION,
  HEADER_LOCATION,
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
  HTTP_UNAUTHORIZED          = 401,
  HTTP_REDIRECT              = 302
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

typedef struct {
  const char* key;
  const char* value;
} QueryParam;

typedef struct {
  const char* name;
  const char* value;
} Cookie;

typedef struct {
  Cookie* cookies;
  int     cookies_count;
} CookieList;

#define MAX_PARAMS 50

typedef struct {
  QueryParam params[MAX_PARAMS];
  int        params_count;
} QueryParamList;

struct HttpRequest {
  char method[8];
  char path[256];
  char version[16];

  struct HttpHeader headers[MAX_HEADERS];
  int               header_count;
  char              body[BODY_LEN];

  JwtData        authenticated_jwt;
  QueryParamList body_query_params_list; // for x-www-form-urlencoded cases
  QueryParamList url_query_params_list;
  CookieList     cookies_list;
};

struct HttpResponse {
  const char*       version;
  HttpStatusCode    status_code;
  const char*       status_text;
  struct HttpHeader headers[MAX_HEADERS];
  char              body[BODY_LEN];
  int               header_count;
};

int         parse_req_line(const char* raw_req, struct HttpRequest* http_req);
const char* parse_req_headers(const char* header_start, struct HttpRequest* http_req);
void        parse_req_body(const char* body_start, struct HttpRequest* http_req);
void        parse_req_cookies(struct HttpRequest* req);
void        get_route_render_template(char* template_content, size_t buf_size, const char* path);
void        urldecode(char* dst, char* encoded_url);
void        append_query_param(char* key, char* value, QueryParamList* params);
void        free_query_params_list(QueryParamList* params);
void        free_cookies_list(CookieList* cookies);

#endif /* CBANK_HTTP_PARSER_H */