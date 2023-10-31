/***************************************************************************//**
 * @brief
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

// *** Generated file. Do not edit! ***
// vNCP Version: 1.0

#include "connect/ember.h"

#include <stdlib.h>
#include <assert.h>
#include "log/log.h"
#include "csp-format.h"
#include "csp-command-utils.h"
#include "connect/callback_dispatcher.h"
#include "csp-api-enum-gen.h"

static void stackStatusCommandHandler(uint8_t *callbackParams)
{
  EmberStatus status;
  fetchCallbackParams(callbackParams,
                      "u",
                      &status);

  emberAfStackStatusCallback(status);
  emberAfStackStatus(status);
}

static void childJoinCommandHandler(uint8_t *callbackParams)
{
  EmberNodeType nodeType;
  EmberNodeId nodeId;
  fetchCallbackParams(callbackParams,
                      "uv",
                      &nodeType,
                      &nodeId);

  emberAfChildJoinCallback(nodeType,
                           nodeId);
  emberAfChildJoin(nodeType,
                   nodeId);
}

static void radioNeedsCalibratingCommandHandler(uint8_t *callbackParams)
{
  (void)callbackParams;

  emberAfRadioNeedsCalibratingCallback();
  emberAfRadioNeedsCalibrating();
}

static void messageSentCommandHandler(uint8_t *callbackParams)
{
  EmberStatus status;
  EmberOutgoingMessage message;
  uint8_t payload[2048];
  message.payload = payload;
  fetchCallbackParams(callbackParams,
                      "uuvuulbuw",
                      &status,
                      &message.options,
                      &message.destination,
                      &message.endpoint,
                      &message.tag,
                      &message.length,
                      message.payload,
                      CSP_FETCH_ARG_IS_UINT16,
                      &message.length,
                      2048,
                      &message.ackRssi,
                      &message.timestamp);

  emberAfMessageSentCallback(status,
                             &message);
  emberAfMessageSent(status,
                     &message);
}

static void incomingMessageCommandHandler(uint8_t *callbackParams)
{
  EmberIncomingMessage message;
  uint8_t payload[2048];
  message.payload = payload;
  fetchCallbackParams(callbackParams,
                      "uvuulbwu",
                      &message.options,
                      &message.source,
                      &message.endpoint,
                      &message.rssi,
                      &message.length,
                      message.payload,
                      CSP_FETCH_ARG_IS_UINT16,
                      &message.length,
                      2048,
                      &message.timestamp,
                      &message.lqi);

  emberAfIncomingMessageCallback(&message);
  emberAfIncomingMessage(&message);
}

static void incomingMacMessageCommandHandler(uint8_t *callbackParams)
{
  EmberIncomingMacMessage message;
  uint8_t eui64Size = EUI64_SIZE;
  uint8_t payload[2048];
  message.payload = payload;
  fetchCallbackParams(callbackParams,
                      "uvbuvbuvvuuuuwlbw",
                      &message.options,
                      &message.macFrame.srcAddress.addr.shortAddress,
                      message.macFrame.srcAddress.addr.longAddress,
                      CSP_FETCH_ARG_IS_UINT8,
                      &eui64Size,
                      EUI64_SIZE,
                      &message.macFrame.srcAddress.mode,
                      &message.macFrame.dstAddress.addr.shortAddress,
                      message.macFrame.dstAddress.addr.longAddress,
                      CSP_FETCH_ARG_IS_UINT8,
                      &eui64Size,
                      EUI64_SIZE,
                      &message.macFrame.dstAddress.mode,
                      &message.macFrame.srcPanId,
                      &message.macFrame.dstPanId,
                      &message.macFrame.srcPanIdSpecified,
                      &message.macFrame.dstPanIdSpecified,
                      &message.rssi,
                      &message.lqi,
                      &message.frameCounter,
                      &message.length,
                      message.payload,
                      CSP_FETCH_ARG_IS_UINT16,
                      &message.length,
                      2048,
                      &message.timestamp);

  emberAfIncomingMacMessageCallback(&message);
  emberAfIncomingMacMessage(&message);
}

static void macMessageSentCommandHandler(uint8_t *callbackParams)
{
  EmberStatus status;
  EmberOutgoingMacMessage message;
  uint8_t eui64Size = EUI64_SIZE;
  uint8_t payload[2048];
  message.payload = payload;
  fetchCallbackParams(callbackParams,
                      "uuvbuvbuvvuuuwlbuw",
                      &status,
                      &message.options,
                      &message.macFrame.srcAddress.addr.shortAddress,
                      message.macFrame.srcAddress.addr.longAddress,
                      CSP_FETCH_ARG_IS_UINT8,
                      &eui64Size,
                      EUI64_SIZE,
                      &message.macFrame.srcAddress.mode,
                      &message.macFrame.dstAddress.addr.shortAddress,
                      message.macFrame.dstAddress.addr.longAddress,
                      CSP_FETCH_ARG_IS_UINT8,
                      &eui64Size,
                      EUI64_SIZE,
                      &message.macFrame.dstAddress.mode,
                      &message.macFrame.srcPanId,
                      &message.macFrame.dstPanId,
                      &message.macFrame.srcPanIdSpecified,
                      &message.macFrame.dstPanIdSpecified,
                      &message.tag,
                      &message.frameCounter,
                      &message.length,
                      message.payload,
                      CSP_FETCH_ARG_IS_UINT16,
                      &message.length,
                      2048,
                      &message.ackRssi,
                      &message.timestamp);

  emberAfMacMessageSentCallback(status,
                                &message);
  emberAfMacMessageSent(status,
                        &message);
}

static void incomingBeaconCommandHandler(uint8_t *callbackParams)
{
  EmberPanId panId;
  EmberMacAddress source;
  int8_t rssi;
  bool permitJoining;
  uint8_t beaconFieldsLength;
  uint8_t beaconPayloadLength;
  uint8_t eui64Size = EUI64_SIZE;
  static uint8_t beaconFields[EMBER_MAC_MAX_BEACON_FIELDS_LENGTH];
  static uint8_t beaconPayload[EMBER_MAC_STACK_BEACON_PAYLOAD_LENGTH + EMBER_MAC_MAX_APP_BEACON_PAYLOAD_LENGTH];
  fetchCallbackParams(callbackParams,
                      "vvbuuuubub",
                      &panId,
                      &source.addr.shortAddress,
                      source.addr.longAddress,
                      CSP_FETCH_ARG_IS_UINT8,
                      &eui64Size,
                      EUI64_SIZE,
                      &source.mode,
                      &rssi,
                      &permitJoining,
                      &beaconFieldsLength,
                      beaconFields,
                      CSP_FETCH_ARG_IS_UINT8,
                      &beaconFieldsLength,
                      EMBER_MAC_MAX_BEACON_FIELDS_LENGTH,
                      &beaconPayloadLength,
                      beaconPayload,
                      CSP_FETCH_ARG_IS_UINT8,
                      &beaconPayloadLength,
                      EMBER_MAC_STACK_BEACON_PAYLOAD_LENGTH + EMBER_MAC_MAX_APP_BEACON_PAYLOAD_LENGTH);

  emberAfIncomingBeaconCallback(panId,
                                &source,
                                rssi,
                                permitJoining,
                                beaconFieldsLength,
                                beaconFields,
                                beaconPayloadLength,
                                beaconPayload);
  emberAfIncomingBeacon(panId,
                        &source,
                        rssi,
                        permitJoining,
                        beaconFieldsLength,
                        beaconFields,
                        beaconPayloadLength,
                        beaconPayload);
}

static void activeScanCompleteCommandHandler(uint8_t *callbackParams)
{
  (void)callbackParams;

  emberAfActiveScanCompleteCallback();
  emberAfActiveScanComplete();
}

static void energyScanCompleteCommandHandler(uint8_t *callbackParams)
{
  int8_t mean;
  int8_t min;
  int8_t max;
  uint16_t variance;
  fetchCallbackParams(callbackParams,
                      "uuuv",
                      &mean,
                      &min,
                      &max,
                      &variance);

  emberAfEnergyScanCompleteCallback(mean,
                                    min,
                                    max,
                                    variance);
  emberAfEnergyScanComplete(mean,
                            min,
                            max,
                            variance);
}

static void frequencyHoppingStartClientCompleteCommandHandler(uint8_t *callbackParams)
{
  EmberStatus status;
  fetchCallbackParams(callbackParams,
                      "u",
                      &status);

  emberAfFrequencyHoppingStartClientCompleteCallback(status);
  emberAfFrequencyHoppingStartClientComplete(status);
}

//------------------------------------------------------------------------------
// Callback command dispatcher (Application side)

void sli_connect_ncp_handle_indication(uint16_t commandId,
                                       uint8_t *callbackParams)
{
  assert(!isCurrentTaskStackTask());

  switch (commandId) {
    case EMBER_STACK_STATUS_HANDLER_IPC_COMMAND_ID:
      stackStatusCommandHandler(callbackParams);
      break;
    case EMBER_CHILD_JOIN_HANDLER_IPC_COMMAND_ID:
      childJoinCommandHandler(callbackParams);
      break;
    case EMBER_RADIO_NEEDS_CALIBRATING_HANDLER_IPC_COMMAND_ID:
      radioNeedsCalibratingCommandHandler(callbackParams);
      break;
    case EMBER_MESSAGE_SENT_HANDLER_IPC_COMMAND_ID:
      messageSentCommandHandler(callbackParams);
      break;
    case EMBER_INCOMING_MESSAGE_HANDLER_IPC_COMMAND_ID:
      incomingMessageCommandHandler(callbackParams);
      break;
    case EMBER_INCOMING_MAC_MESSAGE_HANDLER_IPC_COMMAND_ID:
      incomingMacMessageCommandHandler(callbackParams);
      break;
    case EMBER_MAC_MESSAGE_SENT_HANDLER_IPC_COMMAND_ID:
      macMessageSentCommandHandler(callbackParams);
      break;
    case EMBER_INCOMING_BEACON_HANDLER_IPC_COMMAND_ID:
      incomingBeaconCommandHandler(callbackParams);
      break;
    case EMBER_ACTIVE_SCAN_COMPLETE_HANDLER_IPC_COMMAND_ID:
      activeScanCompleteCommandHandler(callbackParams);
      break;
    case EMBER_ENERGY_SCAN_COMPLETE_HANDLER_IPC_COMMAND_ID:
      energyScanCompleteCommandHandler(callbackParams);
      break;
    case EMBER_FREQUENCY_HOPPING_START_CLIENT_COMPLETE_HANDLER_IPC_COMMAND_ID:
      frequencyHoppingStartClientCompleteCommandHandler(callbackParams);
      break;
    default: {
      FATAL(1, "Unknown incoming callback command id: %04X", commandId);
    }
  }
}
