#ifndef CBANK_EXECUTORS_H
#define CBANK_EXECUTORS_H

#include "select_query.h"

ResultSet* select_executor(SelectQuery*);
void       insert_executor(SelectQuery* q);
void       update_executor(SelectQuery* q);
void       delete_executor(SelectQuery* q);

#endif /* CBANK_EXECUTORS_H */