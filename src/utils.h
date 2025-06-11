#pragma once

#include <stdbool.h>

#define INVALID_CHOICE -1
#define MIN_CHOICE 1
#define MAX_CHOICE 3

int validate_user_id(char* id);
int validate_input_pwd(char* pwd);
int validate_choice(const char* expr);
