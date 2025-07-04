#include "../data/models.h"

Account     mount_acc_from_line_buf(const char* line_buf);
const char* acc_to_line_buf(Account* restrict acc);