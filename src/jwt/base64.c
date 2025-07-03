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

  encoded_data[*output_len] = '\0';

  return encoded_data;
};

char* base64url_encode(const unsigned char* data, size_t input_length) {
  size_t enc_len = 0;
  char*  b64_enc = base64_encode(data, input_length, &enc_len);
  if (!b64_enc)
    return NULL;

  base64_to_base64url(b64_enc);
  return b64_enc;
}

void base64_to_base64url(char* b64) {
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

static const unsigned char b64_dec_table[256] = {
    [0 ... 255] = 0x80, /* (bit 7=1 → 0x80) */['A'] = 0,
    ['B'] = 1,          ['C'] = 2,
    ['D'] = 3,          ['E'] = 4,
    ['F'] = 5,          ['G'] = 6,
    ['H'] = 7,          ['I'] = 8,
    ['J'] = 9,          ['K'] = 10,
    ['L'] = 11,         ['M'] = 12,
    ['N'] = 13,         ['O'] = 14,
    ['P'] = 15,         ['Q'] = 16,
    ['R'] = 17,         ['S'] = 18,
    ['T'] = 19,         ['U'] = 20,
    ['V'] = 21,         ['W'] = 22,
    ['X'] = 23,         ['Y'] = 24,
    ['Z'] = 25,         ['a'] = 26,
    ['b'] = 27,         ['c'] = 28,
    ['d'] = 29,         ['e'] = 30,
    ['f'] = 31,         ['g'] = 32,
    ['h'] = 33,         ['i'] = 34,
    ['j'] = 35,         ['k'] = 36,
    ['l'] = 37,         ['m'] = 38,
    ['n'] = 39,         ['o'] = 40,
    ['p'] = 41,         ['q'] = 42,
    ['r'] = 43,         ['s'] = 44,
    ['t'] = 45,         ['u'] = 46,
    ['v'] = 47,         ['w'] = 48,
    ['x'] = 49,         ['y'] = 50,
    ['z'] = 51,         ['0'] = 52,
    ['1'] = 53,         ['2'] = 54,
    ['3'] = 55,         ['4'] = 56,
    ['5'] = 57,         ['6'] = 58,
    ['7'] = 59,         ['8'] = 60,
    ['9'] = 61,         ['+'] = 62,
    ['/'] = 63,         ['='] = 0};

void base64url_to_b64(char* str) {
  while (*str) {
    if (*str == '-')
      *str = '+';
    else if (*str == '_')
      *str = '/';
    str++;
  }

  size_t len     = strlen(str);
  size_t pad_len = (4 - (len % 4)) % 4;
  for (size_t i = 0; i < pad_len; i++) {
    str[len + i] = '=';
  }
  str[len + pad_len] = '\0';
}

unsigned char* base64decode(char* data, size_t input_length, size_t* output_length) {
  if (input_length % 4 != 0)
    return NULL;

  size_t out_len = input_length / 4 * 3;
  if (data[input_length - 1] == '=')
    out_len--; // remove padding
  if (data[input_length - 2] == '=')
    out_len--;

  unsigned char* decoded_data = malloc(out_len);
  if (!decoded_data)
    return NULL;

  size_t        output_idx = 0;
  unsigned char quad[4];

  for (size_t i = 0; i < input_length;) {
    for (size_t k = 0; k < 4; k++, i++) {
      quad[k] = b64_dec_table[(unsigned char)data[i]];
      if (quad[k] == 0x80) { // invalid character
        free(decoded_data);
        return NULL;
      }
    }

    // byte 1
    decoded_data[output_idx++] =
        (quad[0] << 2) | (quad[1] >> 4); // 6 bits quad[0] + 2 high bits quad[1]
    // byte 2
    if (data[i - 2] != '=')
      decoded_data[output_idx++] =
          (quad[1] << 4) | (quad[2] >> 2); // 4 low bits quad[1] + 2 high bits quad[2]
    // byte 3
    if (data[i - 1] != '=')
      decoded_data[output_idx++] = (quad[2] << 6) | quad[3]; // 2 low bits quad[2] + 6 bits quad[3]
  };

  if (output_length)
    *output_length = output_idx;
  return decoded_data;
}

unsigned char* base64url_decode(char* input, size_t* output_length) {
  base64url_to_b64(input);
  return base64decode(input, strlen(input), output_length);
}