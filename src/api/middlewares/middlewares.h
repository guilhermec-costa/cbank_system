#include "../../server/http_utils.h"

void auth_middleware(struct HttpRequest* req, struct HttpResponse* res);
void decode_url_middleware(struct HttpRequest* req, struct HttpResponse* res);