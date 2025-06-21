#include "../../data/models.h"

typedef struct {
  char   id[20];
  double balance;
  CREATED_AT;
  UPDATED_AT;
} GetAccountDTO;