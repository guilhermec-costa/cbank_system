#ifndef CBANK_USER_H
#define CBANK_USER_H

#include "../api/dtos/user_models.h"
#include "../api/schemas/register_account_validation_schema.h"

CreateUserDTO user_dto_from_register_acc_schema(RegisterAccountSchema* schema);
BankUser      make_new_user(CreateUserDTO payload);
bool          email_already_registered(const char* email);

#endif /* CBANK_USER_H */