#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <poll.h>
#include "log/log.h"
#include "connect/ncp.h"
#include "csp/csp-format.h"
#include "callback-queue.h"
#include "csp/csp-command-utils.h"

#define MAX_CALLBACK_COMMAND_QUEUE_SIZE 50 //this is set arbitrarily for now
#define MAX_PIPE_QUEUE_SIZE             MAX_CALLBACK_COMMAND_QUEUE_SIZE * MAX_STACK_API_COMMAND_SIZE

static int pipe_fds[2];
static struct pollfd poll_fds;

void sli_init_callback_queue()
{
  if (pipe(pipe_fds) < 0) {
    FATAL(1, "Could not initialize callback queue (can't open pipe)");
  }
  poll_fds.fd = pipe_fds[0];
  poll_fds.events = POLLIN;
}

void sli_connect_ncp_append_callback_command(uint8_t *callback_command, uint16_t command_length)
{
  //add command length at the beginning of the command
  uint8_t pipe_command[command_length + 2];
  emberStoreHighLowInt16u(pipe_command, command_length);
  memcpy(pipe_command + 2, callback_command, command_length);

  write(pipe_fds[1], pipe_command, command_length + 2);

  TRACE(TR_CB_QUEUE, "Appending CB: %s", tr_csp_full(callback_command, command_length));
}

void sl_connect_ncp_handle_pending_callback_commands()
{
  uint8_t pipe_buffer[MAX_PIPE_QUEUE_SIZE];
  ssize_t bytes_to_read = read(pipe_fds[0], pipe_buffer, MAX_PIPE_QUEUE_SIZE);
  TRACE(TR_CB_QUEUE, "%d bytes in callback queue", bytes_to_read);
  uint8_t *finger = pipe_buffer;
  while (bytes_to_read > 0) {
    uint16_t command_length = emberFetchHighLowInt16u(finger);
    //move past the command length indicator
    finger += sizeof(uint16_t);
    //execute the callback
    uint16_t command_id = emberFetchHighLowInt16u(finger);
    TRACE(TR_CB_QUEUE, "Handling CB: %s", tr_csp_full(finger, command_length));
    sli_connect_ncp_handle_indication(command_id, finger + 2);
    //pop the first command from the queue
    finger += command_length;
    bytes_to_read += -(command_length + sizeof(uint16_t));

    FATAL_ON(bytes_to_read < 0, 1, "Missing data in callback queue");
  }
}

EmberStatus sl_connect_ncp_poll_callback_command(int32_t timeout)
{
  int ret = poll(&poll_fds, 1, timeout);

  if (ret > 0) {
    if (poll_fds.revents & POLLIN) {
      sl_connect_ncp_handle_pending_callback_commands();
    }
  }

  if (ret < 0) {
    return EMBER_ERR_FATAL;
  }

  return EMBER_SUCCESS;
}
