/***************************************************************************//**
 * @brief Initialize the library by calling everything needed
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

#include "ncp-host-common.h"
#include "cpc-host.h"
#include "callback-queue.h"

void sl_connect_ncp_init(void)
{
  cpc_host_startup();
  commandMutexInit();
  sli_init_callback_queue();
}
