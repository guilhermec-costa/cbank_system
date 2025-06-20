#include "http_utils.h"

const char* get_content_type_string(ContentType type) {
  switch (type) {
    case CONTENT_TYPE_HTML:
      return "text/html; charset=UTF-8";
    case CONTENT_TYPE_JSON:
      return "application/json; charset=UTF-8";
    case CONTENT_TYPE_PLAIN:
      return "text/plain; charset=UTF-8";
    case CONTENT_TYPE_CSS:
      return "text/css; charset=UTF-8";
    case CONTENT_TYPE_JAVASCRIPT:
      return "application/javascript; charset=UTF-8";
    case CONTENT_TYPE_XML:
      return "application/xml; charset=UTF-8";
    case CONTENT_TYPE_FORM_URLENCODED:
      return "application/x-www-form-urlencoded";
    case CONTENT_TYPE_MULTIPART_FORM_DATA:
      return "multipart/form-data";
    case CONTENT_TYPE_OCTET_STREAM:
      return "application/octet-stream";
    default:
      return "application/octet-stream";
  }
}

const char* get_status_text(HttpStatusCode code) {
  switch (code) {
    case HTTP_OK:
      return "OK";
    case HTTP_CREATED:
      return "Created";
    case HTTP_NO_CONTENT:
      return "No Content";
    case HTTP_BAD_REQUEST:
      return "Bad Request";
    case HTTP_UNAUTHORIZED:
      return "Unauthorized";
    case HTTP_FORBIDDEN:
      return "Forbidden";
    case HTTP_NOT_FOUND:
      return "Not Found";
    case HTTP_METHOD_NOT_ALLOWED:
      return "Method Not Allowed";
    case HTTP_INTERNAL_SERVER_ERROR:
      return "Internal Server Error";
    default:
      return "Unknown";
  }
}

static const char* http_header_field_names[] = {
    "Host", "Content-Type", "Content-Length", "User-Agent", "Accept", "Cookie", "Connection"};

const char* get_header_field_name(const HttpHeaderField field) {
  if (field >= 0 && field < HEADER_COUNT) {
    return http_header_field_names[field];
  }
  return "Unknown";
};

const char* get_header(struct HttpRequest* req, HttpHeaderField header_field) {
  const char* header_name = get_header_field_name(header_field);
  for (int i = 0; i < req->header_count; i++) {
    if (strcasecmp(req->headers[i].key, header_name) == 0) {
      return req->headers[i].value;
    }
  }
  return NULL;
};
