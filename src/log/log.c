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

char *str_bytes(const void *in_start, size_t in_len, const void **in_done, char *out_start, size_t out_len, int opt)
{
  static const char *hex_l = "0123456789abcdef";
  static const char *hex_u = "0123456789ABCDEF";
  const char *hex = hex_l;
  const uint8_t *in = in_start;
  const uint8_t *in_end = in + in_len;
  char *out = out_start;
  char *out_end;

  char *ellipsis = "\0";
  char delim = '\0';
  bool fit = true;

  BUG_ON(!out);
  BUG_ON(!out_len);
  if (opt & UPPER_HEX) {
    hex = hex_u;
  }
  if (opt & DELIM_SPACE) {
    delim = ' ';
  }
  if (opt & DELIM_COLON) {
    delim = ':';
  }
  if (delim && out_len < in_len * 3) {
    fit = false;
  }
  if (!delim && out_len < in_len * 2 + 1) {
    fit = false;
  }

  if (!fit) {
    if (opt & ELLIPSIS_ABRT) {
      BUG("buffer is too small");
    }
    if (opt & ELLIPSIS_STAR) {
      ellipsis = "*";
    }
    if (opt & ELLIPSIS_DOTS) {
      ellipsis = "...";
    }
  }

  // Input buffer is null
  if (!in) {
    strncpy(out, "<null>", out_len - 1);
    goto out;
  }

  // Nothing to display just return empty string
  if (!in_len) {
    out[0] = '\0';
    goto out;
  }

  // We can't write at least one byte
  if (out_len <= strlen(ellipsis) + 3) {
    strncpy(out, ellipsis, out_len - 1);
    goto out;
  }

  // Keep one byte for '\0'
  out_end = out + out_len - strlen(ellipsis) - 1;
  while (true) {
    *out++ = hex[*in >> 4];
    *out++ = hex[*in & 0xF];
    in++;
    if (in == in_end) {
      break;
    }
    if (delim && out_end - out < 3) {
      break;
    }
    if (!delim && out_end - out < 2) {
      break;
    }
    if (delim) {
      *out++ = delim;
    }
  }
  strcpy(out, ellipsis);

  out:
  out_start[out_len - 1] = '\0';
  if (in_done) {
    *in_done = in;
  }
  return out_start;
}

char *str_csp_id(uint16_t in, char out[static STR_MAX_LEN_CSP_ID])
{
  sprintf(out, "Command ID: 0x%04X", in);
  return out;
}

char *str_csp_full(const uint8_t *in, int in_len, char *out, int out_len)
{
  return str_bytes(in, in_len, NULL, out, out_len, DELIM_COLON);
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

const char *tr_csp_full(const uint8_t in[], int in_len)
{
  char *out = trace_buffer + trace_idx;

  if (trace_idx + in_len * 3 > sizeof(trace_buffer)) {
    return "[OVERFLOW]";
  }
  str_csp_full(in, in_len, out, in_len * 3);
  trace_idx += strlen(out) + 1;
  BUG_ON(trace_idx > sizeof(trace_buffer));
  return out;
}
