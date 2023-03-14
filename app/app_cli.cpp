/***************************************************************************//**
 * @file
 * @brief CLI handlers for Connect NCP host app
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

#include <cli/cli.h>
#include <cstring>

#include "stack/include/error-def.h"
#include "stack/include/ember-types.h"
#include "include/lib-connect-ncp.h"

#include "app_cli.h"
#include "app_common.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------
/// Literal constant for print operations
#define ENABLED  "enabled"
#define DISABLED "disabled"
/// The destination endpoint of the outgoing message
#define DATA_ENDPOINT           1
#define TX_TEST_ENDPOINT        2

// -----------------------------------------------------------------------------
//                                Static Variables
// -----------------------------------------------------------------------------
/// Sink TX power set by CLI command
static int16_t tx_power = SL_SENSOR_SINK_TX_POWER;

/// Connect security key (default)
EmberKeyData security_key = { .contents = SL_SENSOR_SINK_SECURITY_KEY };

long hexToInt(std::string hex)
{
  if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
    hex.erase(0, 2); //remove 0x header for strtol
  }
  char *p;
  long n = strtol(hex.c_str(), &p, 16);
  if ( *p != 0 ) {
    printf("\nBad argument, enter a hex value");
    return 0;
  }
  return n;
}

unsigned int splitStringIntoHexArray(std::string str, uint8_t *array)
{
  if (str[0] == '{') { // remove '{' '}' delimiters
    str.erase(0, 1);
    str.pop_back();
  }
  for (unsigned int i = 0; i < str.length(); i += 2) {
    std::string s = std::string() + str[i] + str[i + 1];
    array[i / 2] = hexToInt(s) & 0xFF;
  }
  return(str.length() / 2);
}

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------
/******************************************************************************
 * CLI - form command
 * Form a network on the specified channel.
 *****************************************************************************/

void cli_form(std::ostream&, uint16_t radioChannel)
{
  EmberStatus status;
  EmberNetworkParameters parameters;
  uint16_t channel = radioChannel;
  uint16_t default_channel = emberGetDefaultChannel();

  if (channel < default_channel) {
    printf("Channel %d is invalid, the first valid channel is %d!\n", channel, default_channel);
    return;
  }

  memset(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.radioTxPower = tx_power;
  parameters.radioChannel = radioChannel;
  parameters.panId = SL_SENSOR_SINK_PAN_ID;

  status = emberFormNetwork(&parameters);

  printf("form 0x%02X\n", status);
}

/******************************************************************************
 * CLI - permit join
 * Allows sensors to join the sink for a given seconds (or unlimited = 0xff)
 *****************************************************************************/
void cli_pjoin(std::ostream&, std::string durationHex)
{
  EmberStatus status;
  uint8_t duration = hexToInt(durationHex);

  emberClearSelectiveJoinPayload();

  status = emberPermitJoining(duration);
  if (status != EMBER_SUCCESS) {
    printf("Permit join status: 0x%02X", status);
  }
}

/******************************************************************************
 * CLI - set TX power
 * Set antenna power in 0.1 dBm resolution
 *****************************************************************************/
void cli_set_tx_power(std::ostream&, int16_t power)
{
  tx_power = power;

  if (emberSetRadioPower(tx_power, false) == EMBER_SUCCESS) {
    printf("TX power set: %d\n", (int16_t)emberGetRadioPower());
  } else {
    printf("TX power set failed\n");
  }
}

/******************************************************************************
 * CLI - set TX options
 * Set the option bits for ACKs, security and priority via the binary value given.
 *****************************************************************************/
void cli_set_tx_options(std::ostream&, std::string i_sendOptionsHex)
{
  tx_options = hexToInt(i_sendOptionsHex);
  printf("TX options set: MAC acks %s, security %s, priority %s\n",
         ((tx_options & EMBER_OPTIONS_ACK_REQUESTED) ? "enabled" : "disabled"),
         ((tx_options & EMBER_OPTIONS_SECURITY_ENABLED) ? "enabled" : "disabled"),
         ((tx_options & EMBER_OPTIONS_HIGH_PRIORITY) ? "enabled" : "disabled"));
}

/******************************************************************************
 * CLI - remove child
 * Remove a specific sensor from the child list.
 *****************************************************************************/
void cli_remove_child(std::ostream&,
                      uint8_t mode,
                      std::string shortAddressHex,
                      std::string longAdressHex)
{
  uint16_t shortAddress = hexToInt(shortAddressHex);
  size_t longAdressLength = longAdressHex.size();
  EmberStatus status;
  EmberMacAddress address;

  address.mode = mode;

  if (longAdressLength % 2 == 0) {
    longAdressLength = longAdressLength / 2;
    uint8_t longAddress[longAdressLength];
    longAdressLength = splitStringIntoHexArray(longAdressHex, longAddress);

    if (address.mode == EMBER_MAC_ADDRESS_MODE_SHORT) {
      address.addr.shortAddress = shortAddress;
    } else {
      if (longAdressLength != EUI64_SIZE) {
        printf("Wrong param length\n");
        return;
      } else {
        memcpy(address.addr.longAddress, longAddress, EUI64_SIZE);
      }
    }
  }

  status = emberRemoveChild(&address);

  printf("Child removal 0x%02X\n", status);
}

/******************************************************************************
 * CLI - info command
 * It lists the main attributes of the current state of the node
 *****************************************************************************/
void cli_info(std::ostream&)
{
  uint8_t* eui64 = emberGetEui64();

  const char* is_ack = ((tx_options & EMBER_OPTIONS_ACK_REQUESTED) ? ENABLED : DISABLED);
  const char* is_security = ((tx_options & EMBER_OPTIONS_SECURITY_ENABLED) ? ENABLED : DISABLED);
  const char* is_high_prio = ((tx_options & EMBER_OPTIONS_HIGH_PRIORITY) ? ENABLED : DISABLED);

  printf("Info:\n");
  printf("         Node EUI64: ");
  printBuffer(emberGetEui64(), EUI64_SIZE);
  printf("  Network state: 0x%02X\n", emberNetworkState());
  printf("      Node type: 0x%02X\n", emberGetNodeType());
  printf("          eui64: >%x%x%x%x%x%x%x%x\n",
               eui64[7],
               eui64[6],
               eui64[5],
               eui64[4],
               eui64[3],
               eui64[2],
               eui64[1],
               eui64[0]);
  printf("        Node id: 0x%04X\n", emberGetNodeId());
  printf("   Node long id: 0x");
  for (uint8_t i = 0; i < EUI64_SIZE; i++) {
    printf("%02X", emberGetEui64()[i]);
  }
  printf("\n");
  printf("         Pan id: 0x%04X\n", emberGetPanId());
  printf("        Channel: %d\n", (uint16_t)emberGetRadioChannel());
  printf("          Power: %d\n", (int16_t)emberGetRadioPower());
  printf("     TX options: MAC acks %s, security %s, priority %s\n",
         is_ack,
         is_security,
         is_high_prio);

}

/******************************************************************************
 * CLI - leave command
 * By this API call the node forgets the current network and reverts to
 * a network status of EMBER_NO_NETWORK
 *****************************************************************************/
void cli_leave(std::ostream&)
{
  emberResetNetworkState();
}

/******************************************************************************
 * CLI - data command
 * The node sends message to the given destination ID
 *****************************************************************************/
void cli_data(std::ostream&, std::string destinationHex, std::string payload)
{
  EmberStatus status;
  EmberNodeId destination = hexToInt(destinationHex);
  size_t payload_length = payload.size();

  if (payload_length % 2 == 0) {
    uint8_t message_payload[payload_length / 2];
    payload_length = splitStringIntoHexArray(payload, message_payload);

    status = emberMessageSend(destination,
                              DATA_ENDPOINT,
                              0, // messageTag
                              payload_length,
                              message_payload,
                              tx_options);

    printf("TX: Data to 0x%04X:{", destination);
    printBuffer(message_payload, payload_length);
    printf("}: status=0x%02X\n", status);
  } else {
    printf("\nError : Payload length is not even");
  }
}

/******************************************************************************
 * CLI - set_channel command
 * Sets the channel
 *****************************************************************************/
void cli_set_channel(std::ostream&, uint16_t radioChannel)
{
  EmberStatus status = emberSetRadioChannelExtended(radioChannel, false);
  if (status == EMBER_SUCCESS) {
    printf("Radio channel set, status=0x%02X\n", status);
  } else {
    printf("Setting radio channel failed, status=0x%02X\n", status);
  }
}


/******************************************************************************
 * CLI - start_energy_scan command
 * The command scans the energy level on the given channel, e.g.
 * "start_energy_scan 0 10" results in 10 RSSI samples collected on channel 0.
 *****************************************************************************/
void cli_start_energy_scan(std::ostream&,
                           uint16_t channelToScan,
                           uint8_t samples)
{
  EmberStatus status;
  status = emberStartEnergyScan(channelToScan, samples);

  if (status == EMBER_SUCCESS) {
    printf("Start energy scanning: channel %d, samples %d\n", channelToScan, samples);
  } else {
    printf("Start energy scanning failed, status=0x%02X\n", status);
  }
}

/******************************************************************************
 * CLI - set_security_key command
 * The command sets the security key
 *****************************************************************************/
void cli_set_security_key(std::ostream&, std::string keyContents)
{
  (void)keyContents;
  printf("This version of the Host NCP library does not support security.\n");
}

/******************************************************************************
 * CLI - counter command
 * The command prints out the passed stack counter
 *****************************************************************************/
void cli_counter(std::ostream&, uint8_t counterType)
{
  uint32_t counter;
  EmberStatus status = emberGetCounter(counterType, &counter);

  if (status == EMBER_SUCCESS) {
    printf("Counter type=0x%02X: %ld\n", counterType, counter);
  } else {
    printf("Get counter failed, status=0x%02X\n", status);
  }
}


void reset_network_command(std::ostream&)
{
  printf("Resetting the network...");
  emberResetNetworkState();
}
