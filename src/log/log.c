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

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <connect/ember.h>
#include "log.h"

FILE *g_trace_stream = NULL;
unsigned int g_enabled_traces = 0;
bool g_enable_color_traces = true;

static __thread char trace_buffer[256];
static __thread int trace_idx = 0;

char *str_csp_id(uint16_t in, char out[static STR_MAX_LEN_CSP_ID])
{
  sprintf(out, "Command ID: 0x%04X", in);
  return out;
}
/*
 * trace_nested_counter allow to handle nested trace calls. For exemple:
 *  char *a() {
 *      DEBUG();
 *      ...;
 *  }
 *  ...
 *  DEBUG("%d", tr_bytes(...), tr_bytes(a()));
 */
static __thread int trace_nested_counter = 0;

void __tr_enter()
{
  if (!g_trace_stream) {
    g_trace_stream = stdout;
    setlinebuf(stdout);
    g_enable_color_traces = isatty(fileno(g_trace_stream));
  }
  trace_nested_counter++;
}

void __tr_exit()
{
  trace_nested_counter--;
  if (!trace_nested_counter) {
    trace_idx = 0;
  }
}

const char *tr_csp_id(uint16_t in)
{
  char *out = trace_buffer + trace_idx;
  if (trace_idx + STR_MAX_LEN_CSP_ID > sizeof(trace_buffer)) {
    return "[OVERFLOW]";
  }
  str_csp_id(in, out);
  trace_idx += strlen(out) + 1;
  BUG_ON(trace_idx > sizeof(trace_buffer));
  return out;
}
