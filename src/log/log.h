/*
 * Copyright (c) 2021-2022 Silicon Laboratories Inc. (www.silabs.com)
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of the Silicon Labs Master Software License
 * Agreement (MSLA) available at [1].  This software is distributed to you in
 * Object Code format and/or Source Code format and is governed by the sections
 * of the MSLA applicable to Object Code, Source Code and Modified Open Source
 * Code. By using this software, you agree to the terms of the MSLA.
 *
 * [1]: https://www.silabs.com/about-us/legal/master-software-license-agreement
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

extern FILE *g_trace_stream;
extern unsigned int g_enabled_traces;
extern bool g_enable_color_traces;

enum {
  TR_CSP_ID = 0x0001,
  TR_CPC = 0x0002,
};

#define STR_MAX_LEN_CSP_ID     19

#define TRACE(COND, ...)          __TRACE(COND, "" __VA_ARGS__)
#define INFO(...)                 __INFO("" __VA_ARGS__)
#define DEBUG(...)                __DEBUG("" __VA_ARGS__)
#define WARN(...)                 __WARN("" __VA_ARGS__)
#define ERROR(...)                __ERROR("" __VA_ARGS__)
#define FATAL(CODE, ...)          __FATAL(CODE, "" __VA_ARGS__)
#define FATAL_ON(COND, CODE, ...) __FATAL_ON(COND, CODE, "" __VA_ARGS__)
#define BUG(...)                  __BUG("" __VA_ARGS__)
#define BUG_ON(COND, ...)         __BUG_ON(COND, "" __VA_ARGS__)

char *str_csp_id(uint16_t in, char out[STR_MAX_LEN_CSP_ID]);
const char *tr_csp_id(uint16_t in);
void __tr_enter();
void __tr_exit();

#define __TRACE(COND, MSG, ...)                               \
  do {                                                        \
    if (g_enabled_traces & (COND)) {                          \
      if (MSG[0] != '\0') {                                   \
        __PRINT_WITH_TIME(90, MSG, ##__VA_ARGS__); }          \
      else {                                                  \
        __PRINT_WITH_TIME(90, "%s:%d", __FILE__, __LINE__); } \
    }                                                         \
  } while (0)

#define __DEBUG(MSG, ...)                          \
  do {                                             \
    if (MSG[0] != '\0') {                          \
      __PRINT_WITH_LINE(94, MSG, ##__VA_ARGS__); } \
    else {                                         \
      __PRINT_WITH_LINE(94, "trace"); }            \
  } while (0)

#define __WARN(MSG, ...)                             \
  do {                                               \
    if (MSG[0] != '\0') {                            \
      __PRINT(93, "warning: " MSG, ##__VA_ARGS__); } \
    else {                                           \
      __PRINT_WITH_LINE(93, "warning"); }            \
  } while (0)

#define __WARN_ON(COND, MSG, ...)                          \
  ({                                                       \
    bool __ret = (COND);                                   \
    if (__ret) {                                           \
      if (MSG[0] != '\0') {                                \
        __PRINT(93, "warning: " MSG, ##__VA_ARGS__); }     \
      else {                                               \
        __PRINT_WITH_LINE(93, "warning: \"%s\"", #COND); } \
    }                                                      \
    __ret;                                                 \
  })

#define __ERROR(MSG, ...)                          \
  do {                                             \
    if (MSG[0] != '\0') {                          \
      __PRINT(31, "error: " MSG, ##__VA_ARGS__); } \
    else {                                         \
      __PRINT_WITH_LINE(31, "error"); }            \
  } while (0)

#define __FATAL(CODE, MSG, ...)               \
  do {                                        \
    if (MSG[0] != '\0') {                     \
      __PRINT(31, MSG, ##__VA_ARGS__); }      \
    else {                                    \
      __PRINT_WITH_LINE(31, "fatal error"); } \
    exit(CODE);                               \
  } while (0)

#define __FATAL_ON(COND, CODE, MSG, ...)                       \
  do {                                                         \
    if (COND) {                                                \
      if (MSG[0] != '\0') {                                    \
        __PRINT(31, MSG, ##__VA_ARGS__); }                     \
      else {                                                   \
        __PRINT_WITH_LINE(31, "fatal error: \"%s\"", #COND); } \
      exit(CODE);                                              \
    }                                                          \
  } while (0)

#define __INFO(MSG, ...)             \
  do {                               \
    __PRINT(92, MSG, ##__VA_ARGS__); \
  } while (0)

#define __BUG(MSG, ...)                                    \
  do {                                                     \
    if (MSG[0] != '\0') {                                  \
      __PRINT_WITH_LINE(91, "bug: " MSG, ##__VA_ARGS__); } \
    else {                                                 \
      __PRINT_WITH_LINE(91, "bug"); }                      \
    raise(SIGTRAP);                                        \
    __builtin_unreachable();                               \
  } while (0)

#define __BUG_ON(COND, MSG, ...)                             \
  do {                                                       \
    if (COND) {                                              \
      if (MSG[0] != '\0') {                                  \
        __PRINT_WITH_LINE(91, "bug: " MSG, ##__VA_ARGS__); } \
      else {                                                 \
        __PRINT_WITH_LINE(91, "bug: \"%s\"", #COND); }       \
      raise(SIGTRAP);                                        \
      __builtin_unreachable();                               \
    }                                                        \
  } while (0)

#define __PRINT(COLOR, MSG, ...)                                                    \
  do {                                                                              \
    __tr_enter();                                                                   \
    if (COLOR != 0 && g_enable_color_traces) {                                      \
      fprintf(g_trace_stream, "\x1B[" #COLOR "m" MSG "\x1B[0m\n", ##__VA_ARGS__); } \
    else {                                                                          \
      fprintf(g_trace_stream, MSG "\n", ##__VA_ARGS__); }                           \
    __tr_exit();                                                                    \
  } while (0)

#define __PRINT_WITH_TIME(COLOR, MSG, ...)                      \
  do {                                                          \
    struct timespec tp;                                         \
    clock_gettime(CLOCK_REALTIME, &tp);                         \
    __PRINT(COLOR, "%ju.%06ju: " MSG,                           \
            (uintmax_t)tp.tv_sec, (uintmax_t)tp.tv_nsec / 1000, \
            ##__VA_ARGS__);                                     \
  } while (0)

#define __PRINT_WITH_LINE(COLOR, MSG, ...) \
  __PRINT(COLOR, "%s():%d: " MSG, __func__, __LINE__, ##__VA_ARGS__)

#endif
