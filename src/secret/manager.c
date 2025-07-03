#define _POSIX_C_SOURCE 200809L
#include "manager.h"

#include "../server/logger.h"
#include "../utils.h"

#include <stdio.h>
#include <string.h>

static Secrets g_secrets;

void load_env(const char* path) {
  if (g_secrets.secrets)
    return;

  FILE* f = fopen(path, "rb");
  if (!f)
    return;

  int secret_alloc_size      = 20;
  int secret_trigger_realloc = secret_alloc_size - 1;

  g_secrets.secrets = calloc(secret_alloc_size, sizeof(Secret));
  if (!g_secrets.secrets) {
    fclose(f);
    return;
  }
  g_secrets.secrets_count = 0;

  while (true) {
    char* line = read_line(f);
    if (!line) {
      break;
    }

    char* eq = strchr(line, '=');
    if (eq) {
      *eq                      = '\0';
      const char* secret_name  = ltrim(line);
      const char* secret_value = ltrim(eq + 1);
      if (strlen(secret_name) > 0 && strlen(secret_value) > 0) {
        if (g_secrets.secrets_count == secret_trigger_realloc) {
          secret_alloc_size += 20;
          secret_trigger_realloc = secret_alloc_size - 1;
          Secret* _tmp_alloc     = realloc(g_secrets.secrets, sizeof(Secret) * secret_alloc_size);
          if (!_tmp_alloc) {
            g_secrets.secrets_count = 0;
            return;
          }
          g_secrets.secrets = _tmp_alloc;
        }
        g_secrets.secrets[g_secrets.secrets_count].name  = strdup(secret_name);
        g_secrets.secrets[g_secrets.secrets_count].value = strdup(secret_value);
        g_secrets.secrets_count++;
      }
    }
    free(line);
  }
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Environment variables loaded");

  for (int i = 0; i < g_secrets.secrets_count; i++) {
    size_t secret_len    = strlen(g_secrets.secrets[i].value);
    char*  hidden_secret = malloc(secret_len + 1);
    for (size_t i = 0; i < secret_len; i++)
      hidden_secret[i] = '*';
    hidden_secret[secret_len] = '\0';

    printf("\033[0;33m%-20s\033[0m : \033[1;33m%s\033[0m\n", g_secrets.secrets[i].name,
           hidden_secret);
    free(hidden_secret);
  }
  fclose(f);
}

const char* get_env(const char* name) {
  if (!g_secrets.secrets)
    return NULL;
  for (int i = 0; i < g_secrets.secrets_count; i++) {
    Secret secret = g_secrets.secrets[i];
    if (strcmp(name, secret.name) == 0) {
      return secret.value;
    }
  }
  return NULL;
}

void free_secrets() {
  if (!g_secrets.secrets)
    return;

  for (int i = 0; i < g_secrets.secrets_count; i++) {
    free(g_secrets.secrets[i].name);
    free(g_secrets.secrets[i].value);
  }
  GLOBAL_LOGGER->log(GLOBAL_LOGGER, DEBUG, "Environment variables cleaned");
  free(g_secrets.secrets);
}