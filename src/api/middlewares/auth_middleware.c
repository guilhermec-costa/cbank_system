#include "../../jwt/jwt.h"
#include "../../secret/manager.h"
#include "../../server/http_utils.h"
#include "middlewares.h"

void auth_middleware(struct HttpRequest* req, struct HttpResponse* res) {
  const char* token_cookie = get_req_cookie(req, "token");
  if (!token_cookie) {
    make_non_authorized(res);
    return;
  };

  char* payload = jwt_validate(token_cookie, get_env("JWT_SECRET"));
  if (!payload) {
    make_non_authorized(res);
    return;
  }

  JwtData parsed_jwt = parse_jwt_payload(payload);
  if (!parsed_jwt.exp || !parsed_jwt.name || !parsed_jwt.sub) {
    make_non_authorized(res);
    return;
  };
  req->authenticated_jwt = parsed_jwt;
};
