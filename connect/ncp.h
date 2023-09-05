/***************************************************************************//**
 * @brief Stack APIs usable by the app
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement(MSLA); available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __CONNECT_API_H__
#define __CONNECT_API_H__

//------------------------------------------------------------------------------
// Stack specific types, call-able APIs and config
//------------------------------------------------------------------------------

#include "connect/ember.h"

//------------------------------------------------------------------------------
// Connect Host library API
//------------------------------------------------------------------------------

/**
 * @brief
 * Initializes the Connect NCP Host library.
 *
 * It starts the CPC connection with the daemon as well as a the callback queue and an internal Mutex.
 */
void sl_connect_ncp_init(void);

/**
 * @brief
 * Polls the communication with the CPC daemon and dispatches incoming messages to the rest of the library.
 *
 * This API needs to have its own dedicated polling thread, implemented by the user, to prevent blocking the communication with the NCP.
 */
EmberStatus sl_connect_poll_ncp_msg(int32_t timeout);

/**
 * @brief
 * Polls the callback queue to see if any callback command is pending.
 *
 * This is just a poll wrapper of sl_connect_ncp_handle_pending_callback_commands(). Using this API in a thread allows the
 * application to execute any pending callback command as soon as possible.
 */
EmberStatus sl_connect_ncp_poll_callback_command(int32_t timeout);

/**
 * @brief
 * Execute every pending callback command in the queue.
 *
 * This API needs to be called regularly to prevent blocking the callback queue. Once the queue is full, no other callback can be
 * written into it and will thus be irremediably lost.
 */
void sl_connect_ncp_handle_pending_callback_commands();

/**
 * @brief
 * Gets the GSDK version running on the NCP
 */
const char *sl_connect_get_ncp_gsdk_version();

#endif //__CONNECT_API_H__

#ifdef __cplusplus
}
#endif
