#include "wan_infor.h"

const char *valid_protocol_type[] = {
    "ipv4",
    "ipv6",
    "ipv4/ipv6"};

const char *valid_gateway_type[] = {
    "route",
    "bridge"};

const char *valid_vlan_type[] = {
    "tag",
    "untag",
    "transparent"};

const char *valid_link_mode[] = {
    "ipoe",
    "pppoe",
    "bridge"};

const char *valid_ipv4_alloc[] = {
    "dhcp",
    "static"};

const char *valid_ipv6_alloc[] = {
    "slaac",
    "dhcpv6",
    "static"};

const char *valid_prefix_mode[] = {
    "auto",
    "manual"};

static error_t wan_infor_get_name_key(cJSON *json_input, char *name);
static error_t wan_infor_get_pppoe_key(cJSON *json_input, wan_pppoe_t *pppoe);
static error_t wan_infor_get_ipv4_key(cJSON *json_input, wan_ipv4_t *ipv4);
static error_t wan_infor_get_ipv6_key(cJSON *json_input, wan_ipv6_t *ipv6);
static error_t wan_infor_get_vlan_key(cJSON *json_input, wan_vlan_t *vlan);
static error_t wan_infor_get_protocol_key(cJSON *json_input, wan_protocol_t *protocol);
static error_t wan_infor_get_gateway_type_key(cJSON *json_input, wan_gateway_t *gateway_type);
static error_t wan_infor_get_link_mode_key(cJSON *json_input, wan_linkmode_t *link_mode);
static error_t wan_infor_get_mtu_key(cJSON *json_input, uint16_t *mtu);
static error_t wan_infor_get_nat_enable_key(cJSON *json_input, uint8_t *nat_enable);

error_t wan_infor_parser_json(cJSON *json_input, wan_config_t *output)
{
    if (json_input == NULL || output == NULL)
    {
        log_error("Invalid input: json_input or output is NULL");
        return ERROR_INVALID_JSON;
    }

    if (wan_infor_get_name_key(json_input, output->name) != ERROR_NONE)
    {
        log_error("Failed to get name key");
        return ERROR_INVALID_NAME;
    }
    else
    {
        log_debug("Name: %s", output->name);
    }

    if (wan_infor_get_gateway_type_key(json_input, &output->gateway_type) != ERROR_NONE)
    {
        log_error("Failed to get gateway type key");
        return ERROR_INVALID_GATEWAY_TYPE;
    }
    else
    {
        log_debug("Gateway Type: %d", output->gateway_type);
    }

    if (wan_infor_get_protocol_key(json_input, &output->protocol) != ERROR_NONE)
    {
        log_error("Failed to get protocol type key");
        return ERROR_INVALID_PROTOCOL_TYPE;
    }
    else
    {
        log_debug("Protocol Type: %d", output->protocol);
    }

    if (wan_infor_get_vlan_key(json_input, &output->vlan) != ERROR_NONE)
    {
        log_error("Failed to get vlan key");
        return ERROR_INVALID_VLAN;
    }
    else
    {
        log_debug("VLAN Type: %s", (output->vlan.type == 0 ? "tag" : (output->vlan.type == 1 ? "untag" : "transparent")));
        log_debug("VLAN ID: %d", output->vlan.id);
    }

    if (wan_infor_get_mtu_key(json_input, &output->mtu) != ERROR_NONE)
    {
        log_error("Failed to get mtu key");
        return ERROR_INVALID_MTU;
    }
    else
    {
        log_debug("MTU: %d", output->mtu);
    }

    if (wan_infor_get_nat_enable_key(json_input, &output->nat_enable) != ERROR_NONE)
    {
        log_error("Failed to get nat_enable key");
        return ERROR_INVALID_NAT;
    }
    else
    {
        log_debug("NAT Enable: %d", output->nat_enable);
    }

    if (wan_infor_get_link_mode_key(json_input, &output->link_mode) != ERROR_NONE)
    {
        log_error("Failed to get link mode key");
        return ERROR_INVALID_LINK_MODE;
    }
    else
    {
        log_debug("Link Mode: %s", output->link_mode == 0 ? "ipoe" : (output->link_mode == 1 ? "pppoe" : "bridge"));
    }

    if (wan_infor_get_pppoe_key(json_input, &output->pppoe) != ERROR_NONE)
    {
        log_error("Failed to get pppoe key");
        return ERROR_INVALID_USERNAME;
    }
    else
    {
        log_debug("PPPoE Username: %s", output->pppoe.username);
        log_debug("PPPoE Password: %s", output->pppoe.password);
    }

    if (wan_infor_get_ipv4_key(json_input, &output->ipv4) != ERROR_NONE)
    {
        log_error("Failed to get ipv4 key");
        return ERROR_INVALID_IP;
    }
    else
    {
        log_debug("IPv4 Alloc: %s", output->ipv4.alloc == 0 ? "dhcp" : (output->ipv4.alloc == 1 ? "static" : "unknown"));
        log_debug("IPv4 Address: %s", output->ipv4.ip);
        log_debug("IPv4 Mask: %s", output->ipv4.mask);
        log_debug("IPv4 Gateway: %s", output->ipv4.gw);
        int i;
        for (i = 0; i < output->ipv4.dns_count; i++)
        {
            log_debug("IPv4 DNS[%d]: %s", i, output->ipv4.dns[i]);
        }
    }

    if (wan_infor_get_ipv6_key(json_input, &output->ipv6) != ERROR_NONE)
    {
        log_error("Failed to get ipv6 key");
        return ERROR_INVALID_IP;
    }
    else
    {
        log_debug("IPv6 Gateway: %s", output->ipv6.gw);
        log_debug("IPv6 Alloc: %s", output->ipv6.alloc == 0 ? "slaac" : (output->ipv6.alloc == 1 ? "dhcpv6" : "static"));
        log_debug("IPv6 Prefix Value: %s", output->ipv6.prefix.prefix_value);
        log_debug("IPv6 Prefix Mode: %s", output->ipv6.prefix.prefix_mode == 0 ? "auto" : (output->ipv6.prefix.prefix_mode == 1 ? "manual" : "unknown"));
        log_debug("IPv6 Lease Time: %s", output->ipv6.prefix.lease_time);
        log_debug("IPv6 Primary Time: %s", output->ipv6.prefix.primary_time);
    }
    return ERROR_NONE;
}

static error_t wan_infor_get_name_key(cJSON *json_input, char *name)
{
    cJSON *name_key = cJSON_GetObjectItem(json_input, "name");
    if (name_key != NULL && cJSON_IsString(name_key))
    {
        SAFE_SNPRINTF(name, MAX_STR_LEN, "%s", name_key->valuestring);
        return ERROR_NONE;
    }
    else
    {
        log_error("Key 'name' not found or not a string");
        return ERROR_INVALID_NAME;
    }
}

static error_t wan_infor_get_gateway_type_key(cJSON *json_input, wan_gateway_t *gateway_type)
{
    cJSON *gateway_type_key = cJSON_GetObjectItem(json_input, "gateway_type");
    if (gateway_type_key != NULL && cJSON_IsString(gateway_type_key))
    {
        int i;
        for (i = 0; i < GW_SIZE; i++)
        {
            if (SAFE_STRCMP(gateway_type_key->valuestring, valid_gateway_type[i]) == 0)
            {
                *gateway_type = (wan_gateway_t)i;
                return ERROR_NONE;
            }
        }
        log_error("Invalid gateway type field in JSON");
        return ERROR_INVALID_GATEWAY_TYPE;
    }
    else
    {
        log_error("Key 'gateway_type' not found or not a string");
        return ERROR_INVALID_JSON;
    }
}

static error_t wan_infor_get_vlan_key(cJSON *json_input, wan_vlan_t *vlan)
{
    cJSON *vlan_type = cJSON_GetObjectItem(json_input, "vlan_type");
    if (vlan_type != NULL && cJSON_IsString(vlan_type))
    {
        int i;
        for (i = 0; i < VLAN_SIZE; i++)
        {
            if (SAFE_STRCMP(vlan_type->valuestring, valid_vlan_type[i]) == 0)
            {
                vlan->type = (wan_vlan_type_t)i;
                break;
            }
        }
        if (vlan->type >= VLAN_SIZE)
        {
            log_error("Invalid vlan type field in JSON");
            return ERROR_INVALID_VLAN;
        }
    }
    else
    {
        log_error("Key 'vlan_type' not found or not a string");
        return ERROR_INVALID_JSON;
    }
    if (vlan->type == VLAN_TAG)
    {
        cJSON *vlan_id = cJSON_GetObjectItem(json_input, "vlan_id");
        if (vlan_id != NULL && cJSON_IsNumber(vlan_id))
        {
            vlan->id = (uint16_t)vlan_id->valueint;
        }
        else
        {
            log_error("Key 'vlan_id' not found or not a number");
            return ERROR_INVALID_JSON;
        }
    }
    return ERROR_NONE;
}

static error_t wan_infor_get_mtu_key(cJSON *json_input, uint16_t *mtu)
{
    cJSON *mtu_key = cJSON_GetObjectItem(json_input, "mtu");
    if (mtu_key != NULL && cJSON_IsNumber(mtu_key))
    {
        *mtu = (uint16_t)mtu_key->valueint;
        return ERROR_NONE;
    }
    else
    {
        log_error("Key 'mtu' not found or not a number");
        return ERROR_INVALID_JSON;
    }
}

static error_t wan_infor_get_nat_enable_key(cJSON *json_input, uint8_t *nat_enable)
{
    cJSON *nat_enable_key = cJSON_GetObjectItem(json_input, "nat");
    if (nat_enable_key != NULL && cJSON_IsBool(nat_enable_key))
    {
        *nat_enable = (uint8_t)nat_enable_key->valueint;
        return ERROR_NONE;
    }
    else
    {
        log_error("Key 'nat_enable' not found or not a boolean");
        return ERROR_INVALID_JSON;
    }
}

static error_t wan_infor_get_link_mode_key(cJSON *json_input, wan_linkmode_t *link_mode)
{
    cJSON *link_mode_key = cJSON_GetObjectItem(json_input, "link_mode");
    if (link_mode_key != NULL && cJSON_IsString(link_mode_key))
    {
        int i;
        for (i = 0; i < LINK_SIZE; i++)
        {
            if (SAFE_STRCMP(link_mode_key->valuestring, valid_link_mode[i]) == 0)
            {
                *link_mode = (wan_linkmode_t)i;
                return ERROR_NONE;
            }
        }
        log_error("Invalid link mode field in JSON");
        return ERROR_INVALID_LINK_MODE;
    }
    else
    {
        log_error("Key 'link_mode' not found or not a string");
        return ERROR_INVALID_JSON;
    }
}

static error_t wan_infor_get_pppoe_key(cJSON *json_input, wan_pppoe_t *pppoe)
{
    cJSON *pppoe_key = cJSON_GetObjectItem(json_input, "pppoe_username");
    if (pppoe_key != NULL && cJSON_IsString(pppoe_key))
    {
        SAFE_SNPRINTF(pppoe->username, sizeof(pppoe->username), "%s", pppoe_key->valuestring);
    }
    else
    {
        log_error("Key 'pppoe_username' not found or not a string");
        return ERROR_INVALID_USERNAME;
    }

    cJSON *pppoe_password_key = cJSON_GetObjectItem(json_input, "pppoe_password");
    if (pppoe_password_key != NULL && cJSON_IsString(pppoe_password_key))
    {
        SAFE_SNPRINTF(pppoe->password, sizeof(pppoe->password), "%s", pppoe_password_key->valuestring);
    }
    else
    {
        log_error("Key 'pppoe_password' not found or not a string");
        return ERROR_INVALID_PASSWORD;
    }
    return ERROR_NONE;
}

static error_t wan_infor_get_ipv4_key(cJSON *json_input, wan_ipv4_t *ipv4)
{
    cJSON *ipv4_alloc_key = cJSON_GetObjectItem(json_input, "ipv4_alloc");
    if (ipv4_alloc_key != NULL && cJSON_IsString(ipv4_alloc_key))
    {
        int i;
        for (i = 0; i < IPV4_SIZE; i++)
        {
            if (SAFE_STRCMP(ipv4_alloc_key->valuestring, valid_ipv4_alloc[i]) == 0)
            {
                ipv4->alloc = (wan_ipv4_alloc_t)i;
                break;
            }
        }
        if (ipv4->alloc >= IPV4_SIZE)
        {
            log_error("Invalid IPv4 alloc field in JSON");
            return ERROR_INVALID_ALLOC_MODE;
        }
    }
    else
    {
        log_error("Key 'ipv4_alloc' not found or not a string");
        return ERROR_INVALID_JSON;
    }
    if (ipv4->alloc == IPV4_STATIC)
    {
        cJSON *ip = cJSON_GetObjectItem(json_input, "ipv4_ip");
        if (ip != NULL && cJSON_IsString(ip) && is_valid_ipv4(ip->valuestring) == ERROR_NONE)
        {

            SAFE_SNPRINTF(ipv4->ip, sizeof(ipv4->ip), "%s", ip->valuestring);
        }
        else
        {
            log_error("Key 'ipv4_ip' not found, not a string or invalid IP");
            return ERROR_INVALID_IP;
        }

        cJSON *mask = cJSON_GetObjectItem(json_input, "ipv4_mask");
        if (mask != NULL && cJSON_IsString(mask) && is_valid_ipv4(mask->valuestring) == ERROR_NONE)
        {
            SAFE_SNPRINTF(ipv4->mask, sizeof(ipv4->mask), "%s", mask->valuestring);
        }
        else
        {
            log_error("Key 'ipv4_mask' not found, not a string or invalid mask");
            return ERROR_INVALID_MASK;
        }

        cJSON *gw = cJSON_GetObjectItem(json_input, "ipv4_gw");
        if (gw != NULL && cJSON_IsString(gw) && is_valid_ipv4(gw->valuestring) == ERROR_NONE)
        {
            SAFE_SNPRINTF(ipv4->gw, sizeof(ipv4->gw), "%s", gw->valuestring);
        }
        else
        {
            log_error("Key 'ipv4_gw' not found, not a string or invalid gw");
            return ERROR_INVALID_GW;
        }

        cJSON *dns = cJSON_GetObjectItem(json_input, "ipv4_dns");
        if (dns != NULL && cJSON_IsArray(dns))
        {
            ipv4->dns_count = 0;
            int i;
            for (i = 0; i < cJSON_GetArraySize(dns) && i < MAX_DNS_SERVERS; i++)
            {
                cJSON *dns_item = cJSON_GetArrayItem(dns, i);
                if (cJSON_IsString(dns_item) && is_valid_ipv4(dns_item->valuestring) == ERROR_NONE)
                {
                    SAFE_SNPRINTF(ipv4->dns[i], sizeof(ipv4->dns[i]), "%s", dns_item->valuestring);
                    ipv4->dns_count++;
                }
                else
                {
                    log_error("Invalid DNS item in JSON");
                    return ERROR_INVALID_DNS;
                }
            }
        }
    }
    else
    {
        ipv4->dns_count = 0;
        SAFE_MEMSET(ipv4->ip, 0, sizeof(ipv4->ip));
        SAFE_MEMSET(ipv4->mask, 0, sizeof(ipv4->mask));
        SAFE_MEMSET(ipv4->gw, 0, sizeof(ipv4->gw));
        SAFE_MEMSET(ipv4->dns, 0, sizeof(ipv4->dns));
    }
    return ERROR_NONE;
}

static error_t wan_infor_get_ipv6_key(cJSON *json_input, wan_ipv6_t *ipv6)
{
    cJSON *ipv6_alloc_key = cJSON_GetObjectItem(json_input, "ipv6_alloc");
    if (ipv6_alloc_key != NULL && cJSON_IsString(ipv6_alloc_key))
    {
        int i;
        for (i = 0; i < IPV6_SIZE; i++)
        {
            if (SAFE_STRCMP(ipv6_alloc_key->valuestring, valid_ipv6_alloc[i]) == 0)
            {
                ipv6->alloc = (wan_ipv6_alloc_t)i;
                break;
            }
        }
        if (ipv6->alloc >= IPV6_SIZE)
        {
            log_error("Invalid IPv6 alloc field in JSON");
            return ERROR_INVALID_ALLOC_MODE;
        }
    }
    else
    {
        log_error("Key 'ipv6_alloc' not found or not a string");
        return ERROR_INVALID_JSON;
    }
    if (ipv6->alloc == IPV6_DHCPV6 || ipv6->alloc == IPV6_STATIC)
    {
        cJSON *gw = cJSON_GetObjectItem(json_input, "ipv6_gw");
        if (gw != NULL && cJSON_IsString(gw) && is_valid_ipv6(gw->valuestring) == ERROR_NONE)
        {
            SAFE_SNPRINTF(ipv6->gw, sizeof(ipv6->gw), "%s", gw->valuestring);
        }
        else
        {
            log_error("Key 'ipv6_gw' not found, not a string or invalid gw");
            return ERROR_INVALID_GW;
        }
    }
    else
    {
        ipv6->dns_count = 0;
        SAFE_MEMSET(ipv6->gw, 0, sizeof(ipv6->gw));
        SAFE_MEMSET(ipv6->dns, 0, sizeof(ipv6->dns));
    }
    cJSON *prefix = cJSON_GetObjectItem(json_input, "ipv6_prefix_mode");
    if (prefix != NULL && cJSON_IsString(prefix))
    {
        int i;
        for (i = 0; i < PREFIX_SIZE; i++)
        {
            if (SAFE_STRCMP(prefix->valuestring, valid_prefix_mode[i]) == 0)
            {
                ipv6->prefix.prefix_mode = (wan_prefix_mode_t)i;
                break;
            }
        }
        if (ipv6->prefix.prefix_mode >= PREFIX_SIZE)
        {
            log_error("Invalid prefix mode field in JSON");
            return ERROR_INVALID_PREFIX;
        }
    }
    else
    {
        log_error("Key 'ipv6_prefix_mode' not found or not a string");
        return ERROR_INVALID_JSON;
    }
    if (ipv6->prefix.prefix_mode == PREFIX_MANUAL)
    {
        cJSON *prefix_value = cJSON_GetObjectItem(json_input, "ipv6_prefix_value");
        if (prefix_value != NULL && cJSON_IsString(prefix_value))
        {
            SAFE_SNPRINTF(ipv6->prefix.prefix_value, sizeof(ipv6->prefix.prefix_value), "%s", prefix_value->valuestring);
        }
        else
        {
            log_error("Key 'ipv6_prefix_value' not found or not a string");
            return ERROR_INVALID_PREFIX;
        }

        cJSON *primary_time = cJSON_GetObjectItem(json_input, "ipv6_prefix_primaryTime");
        if (primary_time != NULL && cJSON_IsString(primary_time))
        {
            SAFE_SNPRINTF(ipv6->prefix.primary_time, sizeof(ipv6->prefix.primary_time), "%s", primary_time->valuestring);
        }
        else
        {
            log_error("Key 'ipv6_primary_time' not found or not a string");
            return ERROR_INVALID_PRIMARY_TIME;
        }

        cJSON *lease_time = cJSON_GetObjectItem(json_input, "ipv6_prefix_leaseTime");
        if (lease_time != NULL && cJSON_IsString(lease_time))
        {
            SAFE_SNPRINTF(ipv6->prefix.lease_time, sizeof(ipv6->prefix.lease_time), "%s", lease_time->valuestring);
        }
        else
        {
            log_error("Key 'ipv6_lease_time' not found or not a string");
            return ERROR_INVALID_LEASE_TIME;
        }
        log_debug("IPv6 Prefix Value: %s", ipv6->prefix.prefix_value);
        log_debug("IPv6 Prefix Primary Time: %s", ipv6->prefix.primary_time);
        log_debug("IPv6 Prefix Lease Time: %s", ipv6->prefix.lease_time);
    }
    else
    {
        SAFE_MEMSET(ipv6->prefix.prefix_value, 0, sizeof(ipv6->prefix.prefix_value));
        SAFE_MEMSET(ipv6->prefix.primary_time, 0, sizeof(ipv6->prefix.primary_time));
        SAFE_MEMSET(ipv6->prefix.lease_time, 0, sizeof(ipv6->prefix.lease_time));
    }
    return ERROR_NONE;
}

static error_t wan_infor_get_protocol_key(cJSON *json_input, wan_protocol_t *protocol)
{
    cJSON *protocol_key = cJSON_GetObjectItem(json_input, "protocol");
    if (protocol_key != NULL && cJSON_IsString(protocol_key))
    {
        int i;
        for (i = 0; i < PROTO_SIZE; i++)
        {
            if (SAFE_STRCMP(protocol_key->valuestring, valid_protocol_type[i]) == 0)
            {
                *protocol = (wan_protocol_t)i;
                return ERROR_NONE;
            }
        }
        log_error("Invalid protocol field in JSON");
        return ERROR_INVALID_PROTOCOL_TYPE;
    }
    else
    {
        log_error("Key 'protocol' not found or not a string");
        return ERROR_INVALID_JSON;
    }
}