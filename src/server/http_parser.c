#include "http_parser.h"
#include <stdio.h>
#include <string.h>

int parse_request_line(const char *req_buf, struct HttpRequest *http_req) {
  const char* first_line_end = strstr(req_buf, CRLF);
  if(first_line_end == NULL) {
    printf("Invalid HTTP request line\n");
    return -1;
  }

  int line_len = first_line_end - req_buf;
  printf("%d", line_len);
  return 0;
}