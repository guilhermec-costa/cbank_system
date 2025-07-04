#include "../../data/user_store.h"
#include "../../render/engine.h"
#include "../../server/http_utils.h"
#include "../../server/router.h"
#include "../../server/templates_constants.h"

#include <stdio.h>

void handle_home(int fd, struct HttpRequest* req, struct HttpResponse* res) {
  const char*            user_id   = req->authenticated_jwt.sub;
  GetUserByFieldResponse user      = get_user_by_id(user_id);
  const char*            template  = load_template_to_string(INDEX_TEMPLATE_PATH);
  TemplateVar            context[] = {{"name", user.user.name}};

  add_body(res, render_template(template, context, 1));
  add_content_type(res, CONTENT_TYPE_HTML);
  add_content_len(res, strlen(res->body));
  make_res_first_line(res, HTTP_OK);
  send_http_response(fd, res);
}
