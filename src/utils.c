#include "utils.h"

#include <crypt.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int validate_choice(const char* choice, int min, int max) {
  int choice_to_i = atoi(choice);
  if (choice_to_i == 0 || choice_to_i < min || choice_to_i > max)
    return INVALID_CHOICE;
  return choice_to_i;
}

void ask_null_terminated_input_str(char* const src, size_t src_size, const char* question) {
  printf("%s", question);
  fgets(src, src_size, stdin);
  src[strcspn(src, "\n")] = '\0';
}

void reset_entity_attr_mem(void* ent, size_t ent_s) {
  memset(ent, 0, ent_s);
}

const char* hash_str(const char* const str, const char* const salt) {
  const char* const hash = crypt(str, salt);
  return hash;
}

const char* gen_acc_id() {
  static char buf[20];
  static int  counter = 0;
  snprintf(buf, sizeof(buf), "ACC%05d", ++counter);

  return buf;
}

struct tm now() {
  time_t    t  = time(NULL);
  struct tm tm = *localtime(&t);
  return tm;
}
#define RESET_ENTITY(entity) reset_entity_attr_mem(entity, sizeof(entity))
