#ifndef BBANK_SECRET_MANAGER_H
#define BBANK_SECRET_MANAGER_H
typedef struct {
  const char* name;
  const char* value;
} Secret;

typedef struct {
  Secret* secrets;
  int     secrets_count;
} Secrets;

void        load_env(const char* path);
const char* get_env(const char* name);
void        free_secrets();

#endif /* BBANK_SECRET_MANAGER_H */