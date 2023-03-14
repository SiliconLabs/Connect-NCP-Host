/***************************************************************************//**
 * @file
 * @brief app_process.c
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#include <stdlib.h>
#include <stdio.h>
#include "stack/include/error-def.h"
#include "stack/include/ember-types.h"
#include "include/lib-connect-ncp.h"
#include "stack/framework/callback_dispatcher.h"
#include <string.h>
#include <assert.h>
#include "app_common.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------
/// TX options set up for the network
EmberMessageOptions tx_options = EMBER_OPTIONS_ACK_REQUESTED;

static uint32_t reportErrorsCount = 0;
static uint32_t pktsDelivered = 0;
static uint32_t ccaFails = 0;
static uint32_t ackFails = 0;
static uint32_t unknownErrors = 0;

static bool fhClientTestOngoing = false;
EmberNodeId fhClientTestServerNodeId = EMBER_NULL_NODE_ID;
EmberPanId fhClientTestServerPanId = 0xFFFF;

static bool mcuIdleAllowed = true;
static bool mcuSleepAllowed = false;

uint32_t calValues = EMBER_CAL_INVALID_VALUE;

static EmberKeyData defaultKey = { { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                                     0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                                     0xAA, 0xAA, 0xAA, 0xAA } };

/**************************************************************************//**
 * This function is called when a message is received.
 *****************************************************************************/
void emberAfIncomingMessageCallback(EmberIncomingMessage *message)
{
  if ((message->endpoint != SL_SENSOR_SINK_ENDPOINT)
      || ((tx_options & EMBER_OPTIONS_SECURITY_ENABLED)
          && !(message->options & EMBER_OPTIONS_SECURITY_ENABLED))) {
    // drop the message if it's not coming from a sensor
    // or if security is required but the message is non-encrypted
    return;
  }

  printf("RX: Data from 0x%04X:", message->source);
  for (int j = 0; j < message->length; j++) {
    printf(" %02X", message->payload[j]);
  }

  int32_t temperature = 0;
  uint32_t humidity  = 0;
  bool temperature_is_negative = false;

  int32_t temperature_decimal = 0;
  uint32_t humidity_decimal  = 0;

  temperature = (int32_t) emberFetchLowHighInt32u(message->payload);
  humidity = emberFetchLowHighInt32u(message->payload + 4);

  if (temperature < 0) {
    temperature_is_negative = true;
  }

  temperature_decimal = abs(temperature) -  (abs(temperature) / 1000) * 1000;
  temperature = abs(temperature / 1000);
  humidity_decimal = humidity -  (humidity / 1000) * 1000;
  humidity = humidity / 1000;

  printf(" Temperature: %s%d.%03dC Humidity: %d.%03d%%\n", (temperature_is_negative ? "-" : "+"), temperature, temperature_decimal, humidity, humidity_decimal);
}

/**************************************************************************//**
 * This function is called to indicate whether an outgoing message was
 * successfully transmitted or to indicate the reason of failure.
 *****************************************************************************/
void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  (void) message;
  if (status != EMBER_SUCCESS) {
    printf("TX: 0x%02X\n", status);
  }
}

/**************************************************************************//**
 * This function is called by the application framework from the stack status
 * handler.
 *****************************************************************************/
void emberAfStackStatusCallback(EmberStatus status)
{
  switch (status) {
    case EMBER_NETWORK_UP:
      printf("Network up\n");
      break;
    case EMBER_NETWORK_DOWN:
      printf("Network down\n");
      break;
    default:
      printf("Stack status: 0x%02X\n", status);
      break;
  }
}

/**************************************************************************//**
 * This handler is invoked when a new child has joined the device.
 *****************************************************************************/
void emberAfChildJoinCallback(EmberNodeType nodeType,
                              EmberNodeId nodeId)
{
  printf("Sensor joined with node ID 0x%04X, node type: 0x%02X\n", nodeId, nodeType);
}

/**************************************************************************//**
 * This function is called in each iteration of the main application loop and
 * can be used to perform periodic functions.
 *****************************************************************************/
void emberAfTickCallback(void)
{
  // Insert any brief, periodic housekeeping here.
}

/**************************************************************************//**
 * This function is called when a frequency hopping client completed the start
 * procedure.
 *****************************************************************************/
void emberAfFrequencyHoppingStartClientCompleteCallback(EmberStatus status)
{
  if (status != EMBER_SUCCESS) {
    printf("FH Client sync failed, status=0x%02X\n", status);
  } else {
    printf("FH Client Sync Success\n");
  }
}

/**************************************************************************//**
 * This function is called when a requested energy scan is complete.
 *****************************************************************************/
void emberAfEnergyScanCompleteCallback(int8_t mean,
                                       int8_t min,
                                       int8_t max,
                                       uint16_t variance)
{
  printf("Energy scan complete, mean=%d min=%d max=%d var=%d\n",
         mean, min, max, variance);
}