#include "wan_testcase.h"
#include <uci.h>
#include <ctype.h>

#define NETWORK_CONFIG "network"
#define INTERFACE_TYPE "interface"

static bool is_valid_section_name(const char *str);
static bool is_section_deleted(struct uci_context *ctx, const char *package, const char *section);

wan_error_t wan_delete(const char *name)
{
    struct uci_context *ctx = NULL;
    struct uci_ptr ptr = {0};
    char uci_path[128];
    char log_buf[256];
    int ret;

    if (!name || !*name || !is_valid_section_name(name))
    {
        log_error("Invalid section name (FAULT_9007)");
        return WAN_INVALID_INPUT;
    }

    ctx = uci_alloc_context();
    if (!ctx)
    {
        log_error("Failed to allocate UCI context (FAULT_9002)");
        return WAN_UCI_ERROR;
    }

    uci_set_confdir(ctx, "/etc/config");
    SAFE_SNPRINTF(uci_path, sizeof(uci_path), "%s.%s", NETWORK_CONFIG, name);
    log_debug("Deleting UCI path: %s", uci_path);

    ret = uci_lookup_ptr(ctx, &ptr, uci_path, true);
    if (ret != UCI_OK || !(ptr.flags & UCI_LOOKUP_COMPLETE))
    {
        SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Failed to find interface '%s' (FAULT_9005)", name);
        log_error("%s", log_buf);
        uci_free_context(ctx);
        return WAN_NOT_FOUND;
    }

    if (!ptr.s || strcmp(ptr.s->type, INTERFACE_TYPE) != 0)
    {
        SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Section '%s' is not a network interface (FAULT_9005)", name);
        log_error("%s", log_buf);
        uci_free_context(ctx);
        return WAN_NOT_WAN;
    }

    ret = uci_delete(ctx, &ptr);
    if (ret != UCI_OK)
    {
        SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Failed to delete section '%s' (FAULT_9003)", name);
        log_error("%s", log_buf);
        uci_free_context(ctx);
        return WAN_UCI_ERROR;
    }

    ret = uci_save(ctx, ptr.p);
    if (ret != UCI_OK)
    {
        SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Failed to save changes for package '%s' (FAULT_9003)", NETWORK_CONFIG);
        log_error("%s", log_buf);
        uci_free_context(ctx);
        return WAN_UCI_ERROR;
    }

    ret = uci_commit(ctx, &ptr.p, false);
    if (ret != UCI_OK)
    {
        SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Failed to commit changes for package '%s' (FAULT_9003)", NETWORK_CONFIG);
        log_error("%s", log_buf);
        uci_free_context(ctx);
        return WAN_UCI_ERROR;
    }

    if (!is_section_deleted(ctx, NETWORK_CONFIG, name))
    {
        SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Failed to delete section '%s' (FAULT_9003)", name);
        log_error("%s", log_buf);
        uci_free_context(ctx);
        return WAN_UCI_ERROR;
    }

    SAFE_SNPRINTF(log_buf, sizeof(log_buf), "Successfully deleted WAN section: %s", name);
    log_debug("%s", log_buf);

    uci_free_context(ctx);
    return WAN_SUCCESS;
}

static bool is_valid_section_name(const char *str)
{
    if (!str || !*str)
        return false;
    for (; *str; str++)
    {
        if (!isalnum((unsigned char)*str) && *str != '_')
        {
            return false;
        }
    }
    return true;
}

static bool is_section_deleted(struct uci_context *ctx, const char *package, const char *section)
{
    struct uci_ptr ptr = {0};
    char uci_path[128];
    SAFE_SNPRINTF(uci_path, sizeof(uci_path), "%s.%s", package, section);
    int ret = uci_lookup_ptr(ctx, &ptr, uci_path, true);
    return (ret != UCI_OK || !(ptr.flags & UCI_LOOKUP_COMPLETE));
}