#ifndef CBANK_JWT_H
#define CBANK_JWT_H

char* generate_jwt(const char* payload, const char* secret);
char* create_jwt_for_user(const char* user_id, const char* name);

typedef struct {
  long        exp;
  const char* sub;
  const char* name;
} JwtData;

JwtData parse_jwt_payload(char* payload);
char*   jwt_validate(const char* token, const char* secret);

#endif /* CBANK_JWT_H */