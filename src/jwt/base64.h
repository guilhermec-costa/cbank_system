#ifndef CBANK_BASE64_H
#define CBANK_BASE64_H

#include <stdlib.h>

char* base64_encode(const unsigned char* data, size_t input_length, size_t* const output_length);
char* b64url_encode(const unsigned char* data, size_t input_length);

#endif /* CBANK_BASE64_H */