#include "http_parser.h"

#include "../utils.h"
#include "http_utils.h"
#include "route_contants.h"
#include "templates_constants.h"

#include <ctype.h>
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

void parse_req_cookies(struct HttpRequest* req) {
  if (req->header_count == 0)
    return;

  int cookies_alloc_size = 20;
  int realloc_trigger    = cookies_alloc_size - 1;

  Cookie* cookies = calloc(cookies_alloc_size, sizeof(Cookie));
  if (!cookies) {
    return;
  }
  CookieList _cookies;
  _cookies.cookies  = cookies;
  req->cookies_list = _cookies;

  char* cookie_header = get_header(req, HEADER_COOKIE);
  if (!cookie_header) {
    free(cookies);
    return;
  }
  int   cookies_idx = 0;
  char* cookie_token;
  char* rest = cookie_header;
  while ((cookie_token = strtok_r(rest, ";", &rest))) {
    char* sep_pos = strchr(cookie_token, '=');
    if (sep_pos) {
      *sep_pos                 = '\0';
      const char* cookie_name  = ltrim(cookie_token);
      const char* cookie_value = ltrim(sep_pos + 1);

      if (cookies_idx >= MAX_COOKIES)
        break;

      if (cookies_idx == realloc_trigger) {
        cookies_alloc_size += 20;
        realloc_trigger          = cookies_alloc_size - 1;
        Cookie* _tmp_reallocated = realloc(cookies, cookies_alloc_size);
        if (!_tmp_reallocated) {
          for (int i = 0; i < cookies_idx; i++) {
            free_cookies_list(&req->cookies_list);
          }
          free(cookies);
          return;
        }
        cookies          = _tmp_reallocated;
        _cookies.cookies = cookies;
      }
      req->cookies_list.cookies[cookies_idx].name  = strdup(cookie_name);
      req->cookies_list.cookies[cookies_idx].value = strdup(cookie_value);
      req->cookies_list.cookies_count++;
      cookies_idx++;
    };
  }
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

void urldecode(char* dst, char* src) {
  char a, b;

  while (*src) {
    if ((*src == '%') && // encoded special characters (?, &, @, =)
        ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {
      if (a >= 'a')
        a -= 'a' - 'A'; // lowercase to uppercase
      if (a >= 'A')
        a = a - 'A' + 10;
      else
        a -= '0'; // A-F to 0..9
      if (b >= 'a')
        b -= 'a' - 'A'; // lowercase to uppercase
      if (b >= 'A')
        b = b - 'A' + 10;
      else
        b -= '0'; // A-F to 0..9
      *dst++ = (16 * a) + b;
      src += 3;
    } else if (*src == '+') { // encoded blank space
      *dst++ = ' ';
      src++;
    } else {
      *dst++ = *src++; // normal character
    }
  }
  *dst = '\0';
}

void append_query_param(char* key, char* value, QueryParamList* query_params) {
  if (query_params->params_count + 1 > MAX_PARAMS)
    return;
  query_params->params[query_params->params_count++] = (QueryParam){
      .key   = key,
      .value = value,
  };
};

void free_query_params_list(QueryParamList* params) {
  for (int i = 0; i < params->params_count; i++) {
    free(params->params[i].key);
    free(params->params[i].value);
  };
}

void free_cookies_list(CookieList* cookies) {
  if (!cookies || cookies->cookies)
    return;
  for (int i = 0; i < cookies->cookies_count; i++) {
    free(cookies->cookies[i].name);
    free(cookies->cookies[i].value);
  }
  free(cookies->cookies);
  cookies->cookies       = NULL;
  cookies->cookies_count = 0;
}
