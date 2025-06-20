#include "http_parser.h"

const char* get_content_type_string(ContentType type);
const char* get_status_text(HttpStatusCode code);
const char* get_header_field_name(const HttpHeaderField field);
