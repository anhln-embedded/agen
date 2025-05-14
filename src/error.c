#include "error.h"


error_t is_valid_ipv4(const char *ip)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result != 0 ? ERROR_NONE : ERROR_INVALID_IP;
}
error_t is_valid_ipv6(const char *ip)
{
    struct sockaddr_in6 sa;
    int result = inet_pton(AF_INET6, ip, &(sa.sin6_addr));
    return result != 0 ? ERROR_NONE : ERROR_INVALID_IP;
}

const char *error_to_string(error_t error)
{
    switch (error)
    {
    case ERROR_NONE:
        return "No error";
    case ERROR_INVALID_IP:
        return "Invalid IP address";
    case ERROR_INVALID_MASK:
        return "Invalid subnet mask";
    case ERROR_INVALID_GW:
        return "Invalid gateway address";
    case ERROR_INVALID_DNS:
        return "Invalid DNS server address";
    case ERROR_INVALID_VLAN:
        return "Invalid VLAN ID";
    case ERROR_INVALID_MTU:
        return "Invalid MTU size";
    case ERROR_INVALID_USERNAME:
        return "Invalid username";
    case ERROR_INVALID_PASSWORD:
        return "Invalid password";
    case ERROR_INVALID_PREFIX:
        return "Invalid prefix value";
    case ERROR_INVALID_LEASE_TIME:
        return "Invalid lease time";
    case ERROR_INVALID_PRIMARY_TIME:
        return "Invalid primary time";
    case ERROR_INVALID_ALLOC_MODE:
        return "Invalid allocation mode";
    case ERROR_INVALID_PROTOCOL_TYPE:
        return "Invalid protocol type";
    case ERROR_INVALID_GATEWAY_TYPE:
        return "Invalid gateway type";
    case ERROR_INVALID_LINK_MODE:
        return "Invalid link mode";
    default:
        return "Unknown error";
    }
}