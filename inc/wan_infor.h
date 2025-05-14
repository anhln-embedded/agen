#ifndef __WAN_INFOR_H__
#define __WAN_INFOR_H__

#include <stdint.h>
#include <string.h>
#include <cJSON.h>
#include <arpa/inet.h>
#include "error.h"
#include "log.h"
#include "safe_macros.h"


#define MAX_STR_LEN 16
#define MAX_DNS_SERVERS 2
#define MAX_STR_LEN_USER_PASSWORD 16
#define MAX_STR_LEN_IPV4 16
#define MAX_STR_LEN_IPV6 40

/*Protocol type*/
typedef enum
{
    PROTO_IPV4 = 0,
    PROTO_IPV6,
    PROTO_IPV4_IPV6,
    PROTO_SIZE
} wan_protocol_t;

/*Gateway type*/
typedef enum
{
    GW_ROUTE = 0,
    GW_BRIDGE,
    GW_SIZE
} wan_gateway_t;

/*VLAN type*/
typedef enum
{
    VLAN_TAG = 0,
    VLAN_UNTAG,
    VLAN_TRANSPARENT,
    VLAN_SIZE
} wan_vlan_type_t;

/*link mode*/
typedef enum
{
    LINK_IPOE = 0,
    LINK_PPPOE,
    LINK_BRIDGE,
    LINK_SIZE
} wan_linkmode_t;

/*IPv4 allocation*/
typedef enum
{
    IPV4_DHCP = 0,
    IPV4_STATIC,
    IPV4_SIZE
} wan_ipv4_alloc_t;

/*IPv6 allocation*/
typedef enum
{
    IPV6_SLAAC = 0,
    IPV6_DHCPV6,
    IPV6_STATIC,
    IPV6_SIZE
} wan_ipv6_alloc_t;

typedef enum
{
    PREFIX_AUTO = 0,
    PREFIX_MANUAL,
    PREFIX_SIZE
} wan_prefix_mode_t;

/*PPPoE credentials*/
typedef struct
{
    char username[MAX_STR_LEN_USER_PASSWORD];
    char password[MAX_STR_LEN_USER_PASSWORD];
} wan_pppoe_t;

/*IPv4 config*/
typedef struct
{
    char ip[MAX_STR_LEN_IPV4];
    char mask[MAX_STR_LEN_IPV4];
    char gw[MAX_STR_LEN_IPV4];
    char dns[MAX_DNS_SERVERS][MAX_STR_LEN_IPV4];
    wan_ipv4_alloc_t alloc;
    uint8_t dns_count;
} wan_ipv4_t;

typedef struct
{
    char prefix_value[MAX_STR_LEN];
    char primary_time[MAX_STR_LEN];
    char lease_time[MAX_STR_LEN];
    wan_prefix_mode_t prefix_mode;
} wan_ipv6_prefix_t;

typedef struct
{
    char gw[MAX_STR_LEN_IPV6];
    char dns[MAX_DNS_SERVERS][MAX_STR_LEN_IPV6];
    uint8_t dns_count;
    wan_ipv6_alloc_t alloc;
    wan_ipv6_prefix_t prefix;
} wan_ipv6_t;

typedef struct
{
    wan_vlan_type_t type;
    uint16_t id;
} wan_vlan_t;

// Top-level WAN config
typedef struct
{
    char name[MAX_STR_LEN];
    wan_pppoe_t pppoe;
    wan_ipv4_t ipv4;
    wan_ipv6_t ipv6;
    wan_vlan_t vlan;
    wan_protocol_t protocol;
    wan_gateway_t gateway_type;
    wan_linkmode_t link_mode;
    uint16_t mtu;
    uint8_t nat_enable;
} wan_config_t;

error_t wan_infor_parser_json(cJSON *json_input, wan_config_t *output);

#endif // __WAN_INFOR_H__
