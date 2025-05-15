#include <stdio.h>
#include "wan_testcase.h"
#include "safe_macros.h"
#include "log.h"

int main(int argc, char *argv[]) {
    log_debug("WAN Testcase: Delete WAN section");
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <wan_section_name>\n", argv[0]);
        fprintf(stderr, "Example: %s wan\n", argv[0]);
        return -1;
    }

    wan_error_t ret = wan_delete(argv[1]);
    if(ret != WAN_SUCCESS) {
        log_debug("Failed to delete WAN section: %s", argv[1]);
        return -1;
    }
    log_debug("Successfully deleted WAN section: %s", argv[1]);
    return 0;
}