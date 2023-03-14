/***************************************************************************//**
 * @brief Functions for global Host functionning
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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "csp/csp-format.h"
#include "csp/csp-command-utils.h"
#include "cpc-host.h"

static uint8_t apiCommandData[MAX_STACK_API_COMMAND_SIZE];
static pthread_mutex_t lock;

uint8_t *sendBlockingCommand(uint8_t *apiCommandBuffer)
{
  cpc_tx(apiCommandBuffer, MAX_STACK_API_COMMAND_SIZE);
  uint8_t *resp_buffer = wait_for_response();
  memcpy(apiCommandData, resp_buffer, MAX_STACK_API_COMMAND_SIZE);
  return apiCommandData;
}

void sendResponse(uint8_t *apiCommandBuffer)
{
  cpc_tx(apiCommandBuffer, MAX_STACK_API_COMMAND_SIZE);
}

void sendCallbackCommand(uint8_t *callbackCommandBuffer, uint16_t commandLength)
{
  cpc_tx(callbackCommandBuffer, commandLength);
}

uint8_t *getApiCommandPointer()
{
  return apiCommandData;
}

void acquireCommandMutex(void)
{
  pthread_mutex_lock(&lock);
}

void releaseCommandMutex(void)
{
  pthread_mutex_unlock(&lock);
}

bool isCurrentTaskStackTask(void)
{
  return false;
}

void commandMutexInit(void)
{
  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init has failed\n");
    exit(1);
  }
}
