#ifndef CBANK_HANDLERS_H
#define CBANK_HANDLERS_H

#include "http_parser.h"

void handle_home(struct HttpRequest* req, struct HttpResponse* res);
void handle_accounts(struct HttpRequest* req, struct HttpResponse* res);
void handle_login(struct HttpRequest* req, struct HttpResponse* res);

#endif /* CBANK_HANDLERS_H */