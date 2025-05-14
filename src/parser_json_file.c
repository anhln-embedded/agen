#include "parser_json_file.h"

error_t parser_json_file(const char *filename, cJSON **json_output)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        log_error("Failed to open file: %s", filename);
        return ERROR_INVALID_JSON;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        log_error("Memory allocation failed");
        fclose(file);
        return ERROR_INVALID_JSON;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);

    *json_output = cJSON_Parse(buffer);
    free(buffer);

    if (*json_output == NULL) {
        log_error("Failed to parse JSON: %s", cJSON_GetErrorPtr());
        return ERROR_INVALID_JSON;
    }
    return ERROR_NONE;
}