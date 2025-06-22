#include "../../core/auth.h"
#include "../../data/models.h"
#include "../../server/route_contants.h"
#include "middlewares.h"

bool validate_jwt(const char* auth_header) {
  return true;
}

extern BankUser logged_user;

void auth_middleware(struct HttpRequest* req, struct HttpResponse* res) {
  if (strcmp(logged_user.id, NON_EXISTING_USER_ID_FLAG) == 0) {
    make_res_first_line(res, HTTP_UNAUTHORIZED);

    add_res_header(res, get_header_field_name(HEADER_CONTENT_TYPE),
                   get_content_type_string(CONTENT_TYPE_PLAIN));
    strcpy(res->body, "Unauthorized");
  }
};
