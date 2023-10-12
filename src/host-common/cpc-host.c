/***************************************************************************//**
 * @brief CSP functions for Host NCP
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>
#include "log/log.h"
#include "sl_cpc.h"
#include "cpc-host.h"
#include "csp/csp-format.h"
#include "csp/csp-api-enum-gen.h"
#include "csp/csp-command-utils.h"
#include "connect/byte-utilities.h"
#include "connect/ncp.h"
#include "callback-queue.h"

static cpc_handle_t lib_handle;
static cpc_endpoint_t endpoint;
static volatile bool crash_happened = false;
static uint8_t responseBuffer[4096]; //CPC read needs a buffer size of 4096

struct pollfd ncp_fds;
struct pollfd response_fds;
static int pipe_fds[2];

static void init_file_descriptor(int fd)
{
  ncp_fds.fd = fd;
  ncp_fds.events = POLLIN;

  if (pipe(pipe_fds) < 0) {
    FATAL(1, "Could not initialize NCP response file descriptor (can't open pipe)");
  }
  response_fds.fd = pipe_fds[0];
  response_fds.events = POLLIN;
}

//The reset callback is called in the context of the USR1 signal handler. Same care must be taken as to what is happening as when writing any other signal handler.
static void reset_crash_callback(void)
{
  crash_happened = true;
  FATAL(1, "Communication with CPC daemon has been severed");
}

void cpc_host_startup(void)
{
  int ret;
  INFO("Trying to init cpc...");
  do {
    ret = cpc_init(&lib_handle,
                   NULL,     //default instance name (cpcd_0)
                   false,     //no debug traces in stderr
                   reset_crash_callback);     // user reset_crash_callback. Could be null
    usleep(100000);
  } while (ret != 0);

  INFO("CPC initialized on Host");

  int fd = cpc_open_endpoint(lib_handle,
                             &endpoint,
                             SL_CPC_ENDPOINT_CONNECT,
                             1); // transmit window

  if (fd >= 0) {
    // The endpoint was successfully opened, which means the secondary was ready.

    // We send an initial frame to confirm to the secondary that the host is connected
    // This message is only needed with GSDK version 4.3.X. Version 4.4.0 introduces the
    // CPC connection callback that makes this unlock message useless.
    {
      char undefined_version[10] = "UNDEFINED";
      char gsdk_message_version[5] = "4.3.";
      const char* current_gsdk_version = sl_connect_get_ncp_gsdk_version();
      if ((memcmp(current_gsdk_version, undefined_version, sizeof(undefined_version)) == 0)
          || (memcmp(current_gsdk_version, gsdk_message_version, sizeof(gsdk_message_version) - 1) == 0)) {
        uint32_t magic_value = 0xDEADBEEFu;
        size_t size;

        size = cpc_write_endpoint(endpoint,
                                  &magic_value,
                                  sizeof(magic_value),
                                  0); // No flags
        (void) size;
      }

      memset(responseBuffer, 0, sizeof(responseBuffer));
      // Set the file descriptor and start the ncp message thread
      init_file_descriptor(fd);
    }
  } else {
    FATAL(1, "Secondary endpoint not opened");
  }
}

int cpc_tx(const void *buf, unsigned int buf_len)
{
  TRACE(TR_CSP_FULL, "CPC TX: %s", tr_csp_full(buf, buf_len));
  TRACE(TR_CSP_ID, "CPC TX: %s", tr_csp_id(emberFetchHighLowInt16u(buf)));
  return cpc_write_endpoint(endpoint, buf, buf_len, 0);
}

int cpc_rx(void *buf, unsigned int buf_len)
{
  int len = cpc_read_endpoint(endpoint, buf, buf_len, 0);
  if (!len || len < 0) {
    FATAL(1, "Secondary can not be reached");
  }
  return len;
}

uint8_t *wait_for_response(void)
{
  uint8_t exp;
  int ret = poll(&response_fds, 1, 1000);
  if (ret > 0) {
    read(pipe_fds[0], &exp, sizeof(uint8_t));
  } else if (ret == 0) {
    read(pipe_fds[0], &exp, sizeof(uint8_t));
    FATAL(1, "NCP response timed out");
  } else {
    BUG("Poll failed in wait_for_response, with status: %d", ret);
  }
  return responseBuffer;
}

void sl_connect_ncp_handle_response(void)
{
  uint8_t resp = 0;
  write(pipe_fds[1], &resp, sizeof(uint8_t));
}

void sl_connect_ncp_poll_cb(void)
{
  uint16_t command_length = cpc_rx(responseBuffer, 4096);
  uint8_t commandOrigin = responseBuffer[0];

  TRACE(TR_CSP_FULL, "CPC RX: %s", tr_csp_full(responseBuffer, command_length));
  TRACE(TR_CSP_ID, "CPC RX: %s", tr_csp_id(emberFetchHighLowInt16u(responseBuffer)));

  switch (commandOrigin) {
    case (VNCP_CMD_ID & 0xFF00) >> 8:
      sl_connect_ncp_handle_response();
      break;
    case (STACK_CALLBACK_ID & 0xFF00) >> 8:
      sli_connect_ncp_append_callback_command(responseBuffer, command_length);
      break;
    default:
      FATAL(1, "Unknown incoming command type");
      break;
  }
}

EmberStatus sl_connect_poll_ncp_msg(int32_t timeout)
{
  int ret = poll(&ncp_fds, 1, timeout);

  if (ret > 0) {
    if (ncp_fds.revents & POLLIN) {
      sl_connect_ncp_poll_cb();
    }
  }

  if (ret < 0) {
    return EMBER_ERR_FATAL;
  }

  return EMBER_SUCCESS;
}

const char *sl_connect_get_ncp_gsdk_version()
{
  return cpc_get_secondary_app_version(lib_handle);
}
