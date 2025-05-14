#ifndef __WAN_TESTCASE_H__
#define __WAN_TESTCASE_H__

#include "wan_infor.h"


typedef enum{
    WAN_ERROR_NONE = 0,
    WAN_ERROR_GET_IPV4,
    WAN_ERROR_GET_IPV6,
    WAN_ERROR_PING_TIMEOUT,
    WAN_ERROR_OTHER
}wan_error_t;

wan_error_t wan_create(wan_config_t *wan_config);
wan_error_t wan_edit(wan_config_t *wan_config);
wan_error_t wan_delete(void);


#endif