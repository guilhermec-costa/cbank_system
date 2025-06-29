#include "jwt.h"

#include "base64.h"

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <stdio.h>
#include <string.h>

char* generate_jwt(const char* payload, const char* secret) {
  // token = b64url_encode(header).b64url_encode(payload).b64url_encode(signature)
  // signature = HMAC_SHA256(secret, (b64url_encode(header).b64url_encode(payload)))
  const char* jwt_header     = "{\"alg\":\"HS256\",\"typ\":\"JWT\"}";
  char*       header_b64url  = b64url_encode((const unsigned char*)jwt_header, strlen(jwt_header));
  char*       payload_b64url = b64url_encode((const unsigned char*)payload, strlen(payload));

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
      b64url_encode(hmac, 32); // 32 bytes, because hmac used sha256 hash function

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
  time_t now = time(NULL);
  long   exp = now + 3600;
  char   payload[512];

  snprintf(payload, sizeof(payload), "{\"sub\":\"%s\",\"name\":\"%s\",\"exp\":%ld}", user_id, name,
           exp);
  char* token = generate_jwt(payload, "CHURROS");
  return token; // must free
}