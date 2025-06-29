#ifndef CBANK_JWT_H
#define CBANK_JWT_H

char* generate_jwt(const char* payload, const char* secret);

#endif /* CBANK_JWT_H */