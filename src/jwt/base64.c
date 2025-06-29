#include "base64.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/";

#define B64_BYTES_P_INPUT_BYTES_GROUP 4
#define B64_INPUT_BYTES_GROUP_SIZE 3
#define _6_BIT_LIMITER 0x3F

// fuck this fucking algorithm. An entire day trying to understand it
char* base64_encode(const unsigned char* data, size_t input_length, size_t* const output_len) {
  *output_len = B64_BYTES_P_INPUT_BYTES_GROUP *
                ((input_length + 2) / B64_INPUT_BYTES_GROUP_SIZE); // +2 to integer ceiling
  char* encoded_data = malloc(*output_len + 1);                    // +1 for \0
  if (!encoded_data)
    return NULL;

  size_t i = 0, j = 0;

  while (i < input_length) {
    uint32_t octet_a = i < input_length ? data[i++] : 0; // byte 1
    uint32_t octet_b = i < input_length ? data[i++] : 0; // byte 2
    uint32_t octet_c = i < input_length ? data[i++] : 0; // byte 3
    uint32_t triple  = (octet_a << 16) + (octet_b << 8) + octet_c;

    encoded_data[j++] = b64_table[(triple >> 18) & _6_BIT_LIMITER]; // 23-18 bits
    encoded_data[j++] = b64_table[(triple >> 12) & _6_BIT_LIMITER]; // 17-12 bits
    encoded_data[j++] = b64_table[(triple >> 6) & _6_BIT_LIMITER];  // 11-6 bits
    encoded_data[j++] = b64_table[triple & _6_BIT_LIMITER];         // 5-0 bits
  }

  // fill the necessary bytes with padding ('=')
  size_t padding = input_length % 3;
  if (padding) {
    for (size_t k = 0; k < 3 - padding; k++) {
      encoded_data[*output_len - 1 - k] = '=';
    };
  }

  encoded_data[*output_len - 1] = '\0';

  return encoded_data;
};

char* b64url_encode(const unsigned char* data, size_t input_length) {
  size_t enc_len = 0;
  char*  b64_enc = base64_encode(data, input_length, &enc_len);
  if (!b64_enc)
    return NULL;

  return b64_enc;
}

void b64_to_b64url(char* b64) {
  while (*b64) {
    if (*b64 == '+')
      *b64 = '-';
    else if (*b64 == '/')
      *b64 = '_';
    b64++;
  }

  // removes '=' padding
  size_t len = strlen(b64);
  while (len > 0 && b64[len - 1] == '=') {
    b64[len] = '\0';
    len--;
  };
}