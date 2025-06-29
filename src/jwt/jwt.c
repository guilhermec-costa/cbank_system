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
  return "";
};