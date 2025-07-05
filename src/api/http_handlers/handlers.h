#ifndef CBANK_HANDLERS_H
#define CBANK_HANDLERS_H

#include "../../server/http_parser.h"

void handle_home(int fd, struct HttpRequest* req, struct HttpResponse* res);

// ACCOUNT HANDLERS
void handle_accounts(int fd, struct HttpRequest* req, struct HttpResponse* res);
void handle_api_accounts(int fd, struct HttpRequest* req, struct HttpResponse* res);
void handle_register_acocunt(int fd, struct HttpRequest* req, struct HttpResponse* res);

// TRANSFER HANDLERS
void handle_transfer(int fd, struct HttpRequest* req, struct HttpResponse* res);

void handle_extract(int fd, struct HttpRequest* req, struct HttpResponse* res);

void handle_cards(int fd, struct HttpRequest* req, struct HttpResponse* res);

void handle_login(int fd, struct HttpRequest* req, struct HttpResponse* res);

void handle_payments(int fd, struct HttpRequest* req, struct HttpResponse* res);

#endif /* CBANK_HANDLERS_H */