#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <unistd.h>

int validate_choice(const char* choice) {
  int choice_to_i = atoi(choice);
  if (choice_to_i == 0 || choice_to_i < MIN_CHOICE ||
      choice_to_i > MAX_CHOICE)
    return INVALID_CHOICE;
  return choice_to_i;
}

void ask_null_terminated_input_str(char* const src, size_t src_size,
                                   const char* question) {
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

#define RESET_ENTITY(entity) reset_entity_attr_mem(entity, sizeof(entity))
