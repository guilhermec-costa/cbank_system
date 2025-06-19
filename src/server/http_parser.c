#include "http_parser.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

enum HTTP_REQ_PARTS { METHOD, PATH, VERSION };

void safe_strncpy(char* dest, const char* src, size_t size) {
  strncpy(dest, src, size);
  dest[size - 1] = '\0';
}

int parse_request_line(const char* req_buf, struct HttpRequest* http_req) {
  const char* first_line_end = strstr(req_buf, CRLF);
  if (first_line_end == NULL) {
    printf("Invalid HTTP request line\n");
    return -1;
  }

  size_t line_len = first_line_end - req_buf;
  char   _tmp_line_buf[512];
  if (line_len >= sizeof(_tmp_line_buf)) {
    printf("Request line too long\n");
    return -1;
  }
  strncpy(_tmp_line_buf, req_buf, line_len);

  char* token;
  char* rest       = _tmp_line_buf;
  int   parse_part = 0;

  while ((token = strtok_r(rest, " ", &rest))) {
    if (parse_part == METHOD) {
      safe_strncpy(http_req->method, token, sizeof(http_req->method));
    } else if (parse_part == PATH) {
      safe_strncpy(http_req->path, token, sizeof(http_req->path));
    } else if (parse_part == VERSION) {
      safe_strncpy(http_req->version, token, sizeof(http_req->version));
    } else {
      break;
    }
    parse_part++;
  }

  if (parse_part != 3) {
    printf("Malformed request line\n");
    return -1;
  }

  return 0; // success
}