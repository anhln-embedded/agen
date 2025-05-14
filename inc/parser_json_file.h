#ifndef __PARSER_JSON_FILE_H__
#define __PARSER_JSON_FILE_H__
#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>
#include "error.h"
#include "log.h"

#define PATH_JSON_FILE "./wan_config.json"
error_t parser_json_file(const char *filename, cJSON **json_output);

#endif // __PARSER_JSON_FILE_H__