#ifndef CBANK_FLOW_H
#define CBANK_FLOW_H

#include <stdbool.h>

static int pre_login_loop_controller  = 0;
static int post_login_loop_controller = 0;

bool pre_login_loop();
void post_login_loop();

#endif /* CBANK_FLOW_H */