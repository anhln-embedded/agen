#include "wan_testcase.h"


wan_error_t wan_create(wan_config_t *wan_config)
{
    if (wan_config == NULL)
    {
        log_error("Invalid input: wan_config is NULL");
        return WAN_ERROR_OTHER;
    }

    // Example of creating a WAN interface
    log_debug("Creating WAN interface with name: %s", wan_config->name);


    return WAN_ERROR_NONE;
}


wan_error_t wan_edit(wan_config_t *wan_config)
{
}