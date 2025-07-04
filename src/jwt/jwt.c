#define _POSIX_C_SOURCE 200809L

#include "jwt.h"

#include "../secret/manager.h"
#include "../utils.h"
#include "base64.h"

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int constant_time_compare(const unsigned char* r, const unsigned char* l, size_t len) {
  unsigned char result = 0;
  for (size_t i = 0; i < len; i++) {
    result |= r[i] ^ l[i];
  }

  return result == 0;
}

char* generate_jwt(const char* payload, const char* secret) {
  const char* jwt_header = "{\"alg\":\"HS256\",\"typ\":\"JWT\"}";
  char* header_b64url    = base64url_encode((const unsigned char*)jwt_header, strlen(jwt_header));
  char* payload_b64url   = base64url_encode((const unsigned char*)payload, strlen(payload));

  if (!header_b64url || !payload_b64url)
    return NULL;

  size_t message_len = strlen(header_b64url) + 1 /*dot char*/ + strlen(payload_b64url);
  char*  message     = malloc(message_len);
  sprintf(message, "%s.%s", header_b64url, payload_b64url);

  unsigned char* hmac = HMAC(EVP_sha256(), secret, strlen(secret), (unsigned char*)message,
                             strlen(message), NULL, NULL);

  if (!hmac)
    return NULL;

  char* signature_b64url =
      base64url_encode(hmac, 32); // 32 bytes, because hmac used sha256 hash function

  size_t token_len = strlen(message) + 1 /*dot char*/ + strlen(signature_b64url);
  char*  token     = malloc(token_len + 1);
  sprintf(token, "%s.%s", message, signature_b64url);

  free(header_b64url);
  free(payload_b64url);
  free(message);
  free(signature_b64url);

  return token;
};

char* create_jwt_for_user(const char* user_id, const char* name) {
  time_t      now   = time(NULL);
  const char* exp   = get_env("JWT_EXP");
  long        l_exp = exp ? atoi(exp) : 3600;
  char        payload[512];

  snprintf(payload, sizeof(payload), "{\"sub\":\"%s\",\"name\":\"%s\",\"exp\":%ld}", user_id, name,
           now + l_exp);
  char* token = generate_jwt(payload, get_env("JWT_SECRET"));
  return token; // must free
};

char* jwt_validate(const char* token, const char* secret) {
  char* token_cp = strdup(token);
  if (!token_cp)
    return NULL;

  char* header_b64    = strtok(token_cp, ".");
  char* payload_b64   = strtok(NULL, ".");
  char* signature_b64 = strtok(NULL, ".");

  if (!header_b64 || !payload_b64 || !signature_b64) {
    free(token_cp);
    return NULL;
  }

  // reconstruct message
  size_t msg_len = strlen(header_b64) + 1 + strlen(payload_b64);
  char*  message = malloc(msg_len + 1);
  if (!message) {
    free(token_cp);
    return NULL;
  }
  sprintf(message, "%s.%s", header_b64, payload_b64);

  size_t sig_len   = 0;
  char*  sig_input = strdup(signature_b64);
  if (!sig_input) {
    free(token_cp);
    free(message);
    return NULL;
  }

  unsigned char* signature = base64url_decode(sig_input, &sig_len);
  free(sig_input);
  if (!signature) {
    free(token_cp);
    free(message);
    return NULL;
  };

  unsigned int hmac_len = 0;
  // hmac should be equal to the ascii version of the signature so the token is valid
  unsigned char* hmac = HMAC(EVP_sha256(), secret, strlen(secret), (unsigned char*)message,
                             strlen(message), NULL, &hmac_len);
  if (!hmac) {
    free(token_cp);
    free(message);
    free(signature);
    return NULL;
  }

  int valid = (hmac_len == sig_len) && constant_time_compare(hmac, signature, sig_len);

  free(token_cp);
  free(message);
  free(signature);

  if (!valid)
    return NULL;

  char* payload_cp = strdup(payload_b64);
  if (!payload_cp)
    return NULL;

  size_t               payload_len     = 0;
  const unsigned char* decoded_payload = base64url_decode(payload_cp, &payload_len);
  if (!decoded_payload)
    return NULL;

  char* payload_str = malloc(payload_len + 1);
  if (!payload_str) {
    free(payload_cp);
    return NULL;
  }

  memcpy(payload_str, decoded_payload, payload_len);
  payload_str[payload_len] = '\0';

  const char* exp_token = strstr(payload_str, "\"exp\":");
  if (!exp_token) {
    free(payload_str);
    return NULL;
  }

  long   exp = atol(exp_token + strlen("\"exp\":"));
  time_t now = time(NULL);
  if (exp < now) {
    free(payload_str);
    return NULL;
  }

  free(payload_cp);
  return payload_str;
}

JwtData parse_jwt_payload(char* payload) {
  JwtData parsed_jwt = {0};

  char* token;
  char* payload_rest    = payload + 1;
  char* close_brace_pos = strchr(payload, '}');
  *close_brace_pos      = '\0';

  while ((token = strtok_r(payload_rest, ",", &payload_rest))) {
    char* colon_pos = strchr(token, ':');
    if (colon_pos) {
      *colon_pos        = '\0';
      const char* key   = remove_char(token, '\"');
      const char* value = remove_char(colon_pos + 1, '\"');
      if (strcmp(key, "sub") == 0) {
        parsed_jwt.sub = value;
      }
      if (strcmp(key, "name") == 0) {
        parsed_jwt.name = value;
      };
      if (strcmp(key, "exp") == 0) {
        parsed_jwt.exp = atol(value);
      };
    }
  }
  return parsed_jwt;
}