#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdint.h>
#include <arpa/inet.h>

typedef enum
{
    ERROR_NONE = 0,
    ERROR_INVALID_NAME,
    ERROR_INVALID_PROTOCOL_TYPE,
    ERROR_INVALID_GW,
    ERROR_INVALID_LINK_MODE,
    ERROR_INVALID_IP,
    ERROR_INVALID_MASK,
    ERROR_INVALID_DNS,
    ERROR_INVALID_VLAN,
    ERROR_INVALID_MTU,
    ERROR_INVALID_NAT,
    ERROR_INVALID_USERNAME,
    ERROR_INVALID_PASSWORD,
    ERROR_INVALID_PREFIX,
    ERROR_INVALID_LEASE_TIME,
    ERROR_INVALID_PRIMARY_TIME,
    ERROR_INVALID_ALLOC_MODE,
    ERROR_INVALID_GATEWAY_TYPE,
    ERROR_INVALID_JSON
} error_t;


const char *error_to_string(error_t error);
error_t is_valid_ipv4(const char *ip);
error_t is_valid_ipv6(const char *ip);

#endif // __ERROR_H__