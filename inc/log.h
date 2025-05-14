/*
 ******************************************************************************
 * @file       log.h
 * @author     Luu Ngoc Anh
 * @date       04/04/2025
 ******************************************************************************
 */

 #ifndef __LOG_H__
 #define __LOG_H__
 
 #include <stdio.h>
 #include <stdarg.h>
 #include <stdbool.h>
 #include <time.h>
 
 #define MAX_LOG_LINE_SIZE 2048
 #define LOG_USE_COLOR
 
 typedef enum
 {
     LOG_LVL_NONE = 0,
     LOG_LVL_ERROR,
     LOG_LVL_WARN,
     LOG_LVL_DEBUG
 } log_level_t;
 
 
 typedef enum
 {
     LOG_STDOUT,
     LOG_FILE,
     LOG_SYSLOG,
 } log_type_t;
 
 typedef struct
 {
     log_level_t log_config_level;
     log_type_t log_config_type;
     char log_config_file[256];
 } log_config_t;
 
 #define log_error(fmt, ...) \
     log_log(LOG_LVL_ERROR, fmt, ##__VA_ARGS__)
 #define log_warn(fmt, ...) \
     log_log(LOG_LVL_WARN, fmt, ##__VA_ARGS__)
 #define log_debug(fmt, ...) \
     log_log(LOG_LVL_DEBUG, fmt, ##__VA_ARGS__)
 
 void log_init(log_config_t *config);
 void log_cleanup(void);
 void log_set_level(log_level_t level);
 void log_set_file(const char *file_path);
 void log_log(log_level_t level, const char *fmt, ...);
 
 #endif
 
 /********************************* END OF FILE ********************************/
 /******************************************************************************/