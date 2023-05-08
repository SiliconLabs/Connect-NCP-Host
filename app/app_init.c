/***************************************************************************//**
 * @file
 * @brief app_init.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <poll.h>
#include <assert.h>
#include <connect/ncp.h>
#include <connect/ember.h>
#include <connect/ember-types.h>
#include <connect/stack-info.h>
#include "app_common.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

static void start_ncp_msg_thread(void);

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------
extern EmberKeyData security_key;

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

void *poll_ncp_msg(void *arg)
{
  while (1) {
    EmberStatus status = sl_connect_poll_ncp_msg(-1);
    assert(status == EMBER_SUCCESS);
  }
}

void *poll_cb_commands(void *arg)
{
  while ((1)) {
    EmberStatus status = sl_connect_ncp_poll_callback_command(-1);
    assert(status == EMBER_SUCCESS);
  }
}

/******************************************************************************
* Application init
******************************************************************************/
void app_init()
{
  sl_connect_ncp_init();
  start_ncp_msg_thread();
  printf("<Power UP>\n");

  emberNetworkInit();
}

/******************************************************************************
* Application framework init callback
******************************************************************************/
void emberAfInitCallback(void)
{
  EmberStatus em_status = EMBER_ERR_FATAL;
  // CLI info message
  printf("Connect Host Sink App");
  // Set the default security key
  em_status = emberSetNcpSecurityKey(&default_security_key.contents,
                                     EMBER_ENCRYPTION_KEY_SIZE);
  if (em_status != EMBER_SUCCESS) {
    printf("Security key setup status 0x%02X\n", em_status);
  }
  // Start up the Connect network
  printf("Connect Host Sink App");
  em_status = emberNetworkInit();
  printf("Network status 0x%02X\n", em_status);
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

static void start_ncp_msg_thread(void)
{
  pthread_t com_thread;
  pthread_t cb_thread;
  pthread_create(&com_thread, NULL, poll_ncp_msg, NULL);
  pthread_create(&cb_thread, NULL, poll_cb_commands, NULL);
}
