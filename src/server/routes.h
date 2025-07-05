#include "../api/http_handlers/handlers.h"
#include "../api/middlewares/middlewares.h"
#include "http_utils.h"
#include "route_contants.h"
#include "router.h"

Route routes[] = {{
                      .path            = INDEX_ROUTE_PATH,
                      .handler         = handle_home,
                      .allowed_methods = {"GET"},
                      .middlewares =
                          {
                              decode_url_middleware,
                          },
                  },
                  {
                      .path            = ACCOUNTS_ROUTE_PATH,
                      .handler         = handle_accounts,
                      .allowed_methods = {"GET"},
                      .middlewares =
                          {
                              decode_url_middleware,
                              auth_middleware,
                          },
                  },
                  {
                      .path            = "/api/accounts",
                      .handler         = handle_api_accounts,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares =
                          {
                              decode_url_middleware,
                              auth_middleware,
                          },
                  },
                  {
                      .path            = LOGIN_ROUTE_PATH,
                      .handler         = handle_login,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares =
                          {
                              decode_url_middleware,
                          },
                  },
                  {
                      .path            = REGISTER_ACCOUNT_ROUTE_PATH,
                      .handler         = handle_register_acocunt,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares =
                          {
                              decode_url_middleware,
                          },
                  },
                  {
                      .path            = TRANSFER_ROUTE_PATH,
                      .handler         = handle_transfer,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares     = {decode_url_middleware, auth_middleware},
                  },
                  {
                      .path            = EXTRACT_ROUTE_PATH,
                      .handler         = handle_extract,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares     = {decode_url_middleware, auth_middleware},
                  },
                  {
                      .path            = CARDS_ROUTE_PATH,
                      .handler         = handle_cards,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares     = {decode_url_middleware, auth_middleware},
                  },
                  {
                      .path            = PAYMENTS_ROUTE_PATH,
                      .handler         = handle_payments,
                      .allowed_methods = {"GET", "POST"},
                      .middlewares     = {decode_url_middleware, auth_middleware},
                  }};