#ifndef __WAN_TESTCASE_H__
#define __WAN_TESTCASE_H__

#include "wan_infor.h"
#include "cJSON.h"
#include "uci.h"
#include "log.h"

typedef enum{
    WAN_SUCCESS = 0, 
    WAN_INVALID_INPUT,
    WAN_UCI_ERROR,
    WAN_NOT_FOUND,
    WAN_NOT_WAN
}wan_error_t;

wan_error_t wan_create(wan_config_t *wan_config);
wan_error_t wan_edit(wan_config_t *wan_config);
wan_error_t wan_delete(const char *name);

#endif