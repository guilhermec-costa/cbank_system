#ifndef CBANK_BASE64_H
#define CBANK_BASE64_H

#include <stdlib.h>

char* base64_encode(const unsigned char* data, size_t input_length, size_t* const output_length);
char* base64url_encode(const unsigned char* data, size_t input_length);
unsigned char* base64url_decode(char* input, size_t* output_length);
void           base64_to_base64url(char* b64);

#endif /* CBANK_BASE64_H */