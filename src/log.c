#include <log.h>
#include <pthread.h>

static log_config_t logger_config = {
    .log_config_level = LOG_LVL_DEBUG,
    .log_config_type = LOG_FILE,
    .log_config_file = "application.log"};

const char *log_level_names[] = {
    "NONE",
    "ERROR",
    "WARN",
    "DEBUG"};

#ifdef LOG_USE_COLOR

const char *log_level_colors[] = {
    "\033[0;37m", // NONE - white
    "\033[0;31m", // ERROR - red
    "\033[0;33m", // WARN - yellow
    "\033[0;34m"  // DEBUG - blue
};

#else

const char *log_level_colors[] = {
    "", // NONE - no color
    "", // ERROR - no color
    "", // WARN - no color
    ""  // DEBUG - no color
};
#endif

void log_init(log_config_t *config)
{
    if (config != NULL)
    {
        logger_config = *config;
    }
}
void log_cleanup(void)
{
    if (logger_config.log_config_type == LOG_FILE)
    {
        FILE *file = fopen(logger_config.log_config_file, "w");
        if (file != NULL)
        {
            fclose(file);
        }
    }
}
void log_set_level(log_level_t level)
{
    if (level <= LOG_LVL_NONE)
    {
        logger_config.log_config_level = LOG_LVL_NONE;
    }
    else if (level >= LOG_LVL_DEBUG)
    {
        logger_config.log_config_level = LOG_LVL_DEBUG;
    }
    else
    {
        logger_config.log_config_level = level;
    }
}
void log_set_file(const char *file_path)
{
    if (file_path != NULL)
    {
        snprintf(logger_config.log_config_file, sizeof(logger_config.log_config_file), "%s", file_path);
    }
}
void log_log(log_level_t level, const char *fmt, ...)
{
    if (level > logger_config.log_config_level)
    {
        return;
    }
    va_list args;
    va_start(args, fmt);
    char log_line[MAX_LOG_LINE_SIZE];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", tm_info);
    char log_message[MAX_LOG_LINE_SIZE];
    vsnprintf(log_message, sizeof(log_message), fmt, args);
    va_end(args);

    static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&log_mutex);

    if (logger_config.log_config_type == LOG_STDOUT)
    {
        fprintf(stdout, "[%s] %s%s\033[0m: %s\n", log_line, log_level_colors[level], log_level_names[level], log_message);
    }
    else if (logger_config.log_config_type == LOG_FILE)
    {
        FILE *file = fopen(logger_config.log_config_file, "a");
        if (file != NULL)
        {
            fprintf(file, "[%s] %s: %s\n", log_line, log_level_names[level], log_message);
            fclose(file);
        }
        else
        {
            fprintf(stderr, "Failed to open log file: %s\n", logger_config.log_config_file);
        }
    }
    else if (logger_config.log_config_type == LOG_SYSLOG)
    {
        fprintf(stderr, "Syslog logging not implemented\n");
    }
    else
    {
        fprintf(stderr, "Unknown log type: %d\n", logger_config.log_config_type);
    }
    pthread_mutex_unlock(&log_mutex);
}