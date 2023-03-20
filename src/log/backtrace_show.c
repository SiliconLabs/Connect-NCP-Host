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

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BT_BUFFER_SIZE 100

void backtrace_show()
{
  int nptrs;
  void *bt_buffer[BT_BUFFER_SIZE];
  char **bt_str;

  nptrs = backtrace(bt_buffer, BT_BUFFER_SIZE);

  bt_str = backtrace_symbols(bt_buffer, nptrs);
  if (bt_str == NULL) {
    fprintf(stderr, "Can't get backtrace symbols");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < nptrs; i++) {
    fprintf(stderr, "%s\n", bt_str[i]);
  }

  free(bt_str);
}
