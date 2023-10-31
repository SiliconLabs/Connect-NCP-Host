/***************************************************************************//**
 * @brief CSP functions to use RTOS or NCP depending on the config
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

#ifndef __CPC_HOST_H__
#define __CPC_HOST_H__

/* This ifdef allows the header to be used from both C and C++. */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void cpc_host_startup(void);
int cpc_tx(const void *buf, unsigned int buf_len);
int cpc_rx(void *buf, unsigned int buf_len);
uint8_t *wait_for_response(void);
bool gsdk_version_is_younger_than_v_4_4(void);

#ifdef __cplusplus
}
#endif

#endif
