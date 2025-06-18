#ifndef CBANK_ACC_H
#define CBANK_ACC_H

typedef enum _Transaction_Type {
  DEPOSIT,
  WITHDRAW,
} TransactionType;

double check_user_balance();
int make_transaction_op(double v, TransactionType ttype);

#endif /* CBANK_ACC_H */