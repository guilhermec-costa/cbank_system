#ifndef CBANK_JWT_H
#define CBANK_JWT_H

char* generate_jwt(const char* payload, const char* secret);
char* create_jwt_for_user(const char* user_id, const char* name);

#endif /* CBANK_JWT_H */