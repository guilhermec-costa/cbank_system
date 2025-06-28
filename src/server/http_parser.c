#define _POSIX_C_SOURCE 200809L

#include "http_parser.h"

#include "route_contants.h"
#include "templates_constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum HTTP_REQ_PARTS { METHOD, PATH, VERSION };

static void safe_strncpy(char* dest, const char* src, size_t size) {
  strncpy(dest, src, size);
  dest[size - 1] = '\0';
}

int parse_req_line(const char* req_buf, struct HttpRequest* http_req) {
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
};

const char* parse_req_headers(const char* header_start, struct HttpRequest* http_req) {
  http_req->header_count = 0;
  char        line_buf[1024];
  const char* req_line = header_start;

  while (1) {
    const char* next_h = strstr(req_line, CRLF);
    if (!next_h)
      break;

    size_t line_len = next_h - req_line;
    if (line_len == 0) {
      return next_h + 2; // end of headers
    }

    if (line_len >= sizeof(line_buf))
      line_len = sizeof(line_buf) - 1; // minus one, so it has space to terminate the string
    memcpy(line_buf, req_line, line_len);
    line_buf[line_len] = '\0';

    char* colon = strchr(line_buf, ':');
    if (colon) {
      *colon      = '\0';
      char* key   = line_buf;
      char* value = colon + 1;
      while (*value == ' ')
        value++; // remove blank spaces

      if (http_req->header_count < MAX_HEADERS) {
        strncpy(http_req->headers[http_req->header_count].key, key, MAX_KEY_LEN - 1);
        strncpy(http_req->headers[http_req->header_count].value, value, MAX_VALUE_LEN - 1);
        http_req->header_count++;
      }
    }

    req_line = next_h + 2;
  };

  return req_line;
};

void parse_req_body(const char* req_start, struct HttpRequest* http_req) {
  const char* content_length = get_header(http_req, HEADER_CONTENT_LENGTH);
  if (content_length) {
    int len = atoi(content_length);
    if (len > 0 && len < BODY_LEN) {
      memcpy(http_req->body, req_start, len);
      http_req->body[len] = '\0';
    };
  };
};

struct RoutePathTemplate {
  const char* route_path;
  const char* route_path_template;
};

struct RoutePathTemplate route_path_template_map[] = {
    {NOT_FOUND_ROUTE_PATH, _404_TEMPLATE_PATH},
    {INDEX_ROUTE_PATH, INDEX_TEMPLATE_PATH},
    {LOGIN_ROUTE_PATH, LOGIN_TEMPLATE_PATH},
    {ACCOUNTS_ROUTE_PATH, ACCOUNTS_TEMPLATE_PATH},
    {REGISTER_ACCOUNT_ROUTE_PATH, REGISTER_ACCOUNT_TEMPLATE_PATH}};

void get_route_render_template(char* template_content, size_t buf_size, const char* path) {
  FILE* template_file = NULL;

  for (size_t i = 0; i < sizeof(route_path_template_map) / sizeof(route_path_template_map[0]);
       i++) {
    struct RoutePathTemplate rpt = route_path_template_map[i];
    if (strcmp(path, rpt.route_path) == 0) {
      template_file = fopen(rpt.route_path_template, "r");
    }
  }

  if (!template_file) {
    perror("Failed to open template file");
    return;
  }

  size_t bytes_read            = fread(template_content, 1, buf_size - 1, template_file);
  template_content[bytes_read] = '\0';

  fclose(template_file);
}