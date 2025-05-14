/*
 ******************************************************************************
 * @file       safe_macros.h
 * @author     Luu Ngoc Anh
 * @date       11/04/2025
 * @brief      Safe string and memory operation macros (NULL-checked)
 ******************************************************************************
 */

 #ifndef __SAFE_H__
 #define __SAFE_H__
 
 #include <string.h>
 #include <stddef.h>
 #include <stdio.h>
 
 /* ================= MEMORY OPERATIONS ================= */
 #define SAFE_MEMCHR(str, c, n)           (((str) != NULL) ? memchr((str), (c), (n)) : NULL)
 #define SAFE_MEMCMP(s1, s2, n)           (((s1) && (s2)) ? memcmp((s1), (s2), (n)) : -1)
 #define SAFE_MEMCPY(dest, src, n)        (((dest) && (src)) ? memcpy((dest), (src), (n)) : NULL)
 #define SAFE_MEMMOVE(dest, src, n)       (((dest) && (src)) ? memmove((dest), (src), (n)) : NULL)
 #define SAFE_MEMSET(str, c, n)           (((str) != NULL) ? memset((str), (c), (n)) : (void *)(0))
 
 /* ================= STRING OPERATIONS ================= */
 #define SAFE_STRCAT(dest, src)           (((dest) && (src)) ? strcat((dest), (src)) : NULL)
 #define SAFE_STRNCAT(dest, src, n)       (((dest) && (src)) ? strncat((dest), (src), (n)) : NULL)
 #define SAFE_STRCHR(str, c)              ((str) ? strchr((str), (c)) : NULL)
 #define SAFE_STRCMP(s1, s2)              (((s1) && (s2)) ? strcmp((s1), (s2)) : -1)
 #define SAFE_STRNCMP(s1, s2, n)          (((s1) && (s2)) ? strncmp((s1), (s2), (n)) : -1)
 #define SAFE_STRCOLL(s1, s2)             (((s1) && (s2)) ? strcoll((s1), (s2)) : -1)
 #define SAFE_STRCPY(dest, src)           (((dest) && (src)) ? strcpy((dest), (src)) : NULL)
 #define SAFE_STRNCPY(dest, src, n)       (((dest) && (src)) ? strncpy((dest), (src), (n)) : NULL)
 #define SAFE_STRCSPN(s1, s2)             (((s1) && (s2)) ? strcspn((s1), (s2)) : 0)
 #define SAFE_STRERROR(errnum)            strerror((errnum))
 #define SAFE_STRLEN(str)                 ((str) ? strlen((str)) : 0)
 #define SAFE_STRPBRK(s1, s2)             (((s1) && (s2)) ? strpbrk((s1), (s2)) : NULL)
 #define SAFE_STRRCHR(str, c)             ((str) ? strrchr((str), (c)) : NULL)
 #define SAFE_STRSPN(s1, s2)              (((s1) && (s2)) ? strspn((s1), (s2)) : 0)
 #define SAFE_STRSTR(hay, needle)         (((hay) && (needle)) ? strstr((hay), (needle)) : NULL)
 
 /*
  * WARNING: strtok is not thread-safe. Consider using strtok_r in multithreaded environments.
  */
 #define SAFE_STRTOK(str, delim)          (((str) && (delim)) ? strtok((str), (delim)) : NULL)
 #define SAFE_STRXFRM(dest, src, n)       (((dest) && (src)) ? strxfrm((dest), (src), (n)) : 0)
 
 /* ================= I/O OPERATIONS ================= */
 #define SAFE_FGETS(str, n, stream)       (((str) && (stream)) ? fgets((str), (n), (stream)) : NULL)
 #define SAFE_SSCANF(str, format, ...)    (((str) && (format)) ? sscanf((str), (format), __VA_ARGS__) : -1)
 #define SAFE_FSCANF(stream, format, ...) (((stream) && (format)) ? fscanf((stream), (format), __VA_ARGS__) : -1)
 #define SAFE_FPRINTF(stream, format, ...) (((stream) && (format)) ? fprintf((stream), (format), __VA_ARGS__) : -1)
 
 /* ================= FORMATTED OUTPUT ================= */
 #define SAFE_SNPRINTF(str, size, format, ...) \
     (((str) && (format)) ? snprintf((str), (size), (format), __VA_ARGS__) : -1)
 
 /* Note: Removed SAFE_SPRINTF due to safety concerns. Use SAFE_SNPRINTF instead. */
 
 #endif /* __SAFE_H__ */
 
 /********************************* END OF FILE ********************************/
 