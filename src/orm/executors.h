#ifndef CBANK_EXECUTORS_H
#define CBANK_EXECUTORS_H

#include "query.h"

int          select_executor(SelectQuery*, char**** results, int* rows, int* cols);
void         insert_executor(SelectQuery* q);
void         update_executor(SelectQuery* q);
void         delete_executor(SelectQuery* q);
SelectQuery* new_select_query();

#endif /* CBANK_EXECUTORS_H */