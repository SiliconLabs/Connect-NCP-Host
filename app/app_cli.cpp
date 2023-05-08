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

#include <connect/error-def.h>
#include <connect/ember-types.h>
#include <connect/ncp.h>
#include <connect/stack-info.h>

#include "app_cli.h"
#include "app_common.h"

#include <connect/ota-unicast-bootloader-server.h>
#include "sl_connect_sdk_ota_bootloader_test_common.h"

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

/// Node ID of the target
static EmberNodeId target;

// -----------------------------------------------------------------------------
//                          Static Function Declarations
// -----------------------------------------------------------------------------

/******************************************************************************
* Convert hex string parameter to binary
******************************************************************************/
static long hexToInt(std::string hex);

/******************************************************************************
* Convert hex byte string like e.g. "{012345}" to array { 0x01, 0x23, 0x45 }
******************************************************************************/
static unsigned int splitStringIntoHexArray(std::string str, uint8_t *array);

/**************************************************************************//**
 * Function to read the GBL file from the file system of the host file system.
 *****************************************************************************/
static EmberStatus read_gbl_file(std::string filename,
                                 uint8_t** gbl_image,
                                 uint32_t* gbl_size);

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
  EmberStatus status;
  uint8_t key_length;
  uint8_t key[EMBER_ENCRYPTION_KEY_SIZE];

  key_length = splitStringIntoHexArray(keyContents, key);
  if (key_length != EMBER_ENCRYPTION_KEY_SIZE) {
    printf("Security key length must be: %d bytes\n", EMBER_ENCRYPTION_KEY_SIZE);
    return;
  }

  status = emberSetNcpSecurityKey(key,
                                  EMBER_ENCRYPTION_KEY_SIZE);
  if (status != EMBER_SUCCESS) {
    printf("Security key set failed, status=0x%02X", status);
  } else {
    printf("Security key set successful.\n");
  }
}


/******************************************************************************
 * CLI - set_security_key command
 * The command unsets any security key in effect.
 *****************************************************************************/
void cli_unset_security_key(std::ostream&)
{
  emberRemovePsaSecurityKey();
  printf("Security key unset successful\n");
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

/**************************************************************************//**
 * CLI - bootloader_unicast_set_target command
 * Sets the target UID of the node that should recevie the image transmitted.
 *****************************************************************************/
void cli_bootloader_unicast_set_target(std::ostream&,
                                       std::string target_id_hex)
{
  target = hexToInt(target_id_hex);
  printf("unicast target set\n");
}

/**************************************************************************//**
 * CLI - bootloader_unicast_distribute command
 * Initiates the distribution of the unicast image transmission to the target
 * node.
 *****************************************************************************/
void cli_bootloader_unicast_distribute(std::ostream&,
                                       uint32_t image_size,
                                       std::string image_tag_hex)
{
  uint8_t image_tag = hexToInt(image_tag_hex);

  if (gbl_image == NULL) {
    printf("No GBL image was loaded!\n");
    return;
  }

  EmberAfOtaUnicastBootloaderStatus status =
    emberAfPluginOtaUnicastBootloaderServerInitiateImageDistribution(target,
                                                                     image_size,
                                                                     image_tag);

  if (status == EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS) {
    printf("unicast image distribution initiated\n");
  } else {
    printf("unicast image distribution failed 0x%x\n", status);
  }
}

/**************************************************************************//**
 * CLI - bootloader_unicast_request_bootload command
 * Initiates and schedules a bootload event on the target node, resulting 
 * in flashing the image and re-starting the target node with the new firmware.
 *****************************************************************************/
void cli_bootloader_unicast_request_bootload(std::ostream&,
                                             uint32_t delay_ms,
                                             std::string image_tag_hex)
{
  uint8_t image_tag = hexToInt(image_tag_hex);

  EmberAfOtaUnicastBootloaderStatus status =
    emberAfPluginUnicastBootloaderServerInitiateRequestTargetBootload(delay_ms,
                                                                      image_tag,
                                                                      target);
  if (status == EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS) {
    printf("bootload request initiated\n");
  } else {
    printf("bootload request failed 0x%x\n", status);
  }
}

/**************************************************************************//**
 * CLI - bootloader_set_tag command
 * Sets the image tag, an 8-bit value to identify the image transmitted.
 * The server (host) and the client (target node) must agree in the tag to
 * start the image transmission process.
 *****************************************************************************/
void cli_bootloader_set_tag(std::ostream&,
                            std::string new_image_tag_hex)
{
  uint8_t new_image_tag = hexToInt(new_image_tag_hex);

  if (new_image_tag != ota_bootloader_test_image_tag) {
    ota_bootloader_test_image_tag = new_image_tag;
    ota_resume_start_counter_reset = true;
  }
  printf("image tag set\n");
}

/**************************************************************************//**
 * CLI - load_gbl_file command
 * Loads the GBL file given by its path, into a memory buffer representing the 
 * image slot of the host. Returns the length of the file needed later for the
 * transmission.
 *****************************************************************************/
void cli_load_gbl_file(std::ostream&,
                       std::string filename)
{
  uint32_t gbl_size;

  EmberStatus status = read_gbl_file(filename,
                                     &gbl_image,
                                     &gbl_size);

  if (status == EMBER_BAD_ARGUMENT) {
    printf("Can't find/load GBL file!\n");
  } else if (status == EMBER_ERR_FATAL) {
    printf("Can't allocate memory for GBL file!\n");
  } else if (status == EMBER_SUCCESS) {
    printf("GBL file of %u bytes loaded into memory.\n", gbl_size);
  }
}

// -----------------------------------------------------------------------------
//                          Static Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
* Convert hex string parameter to binary
******************************************************************************/
static long hexToInt(std::string hex)
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

/******************************************************************************
* Convert hex byte string like e.g. "{012345}" to array { 0x01, 0x23, 0x45 }
******************************************************************************/
static unsigned int splitStringIntoHexArray(std::string str, uint8_t *array)
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


/**************************************************************************//**
 * Function to read the GBL file from the file system of the host file system.
 *****************************************************************************/
static EmberStatus read_gbl_file(std::string filename,
                                 uint8_t** gbl_image,
                                 uint32_t* gbl_size)
{
  FILE* gbl_file_hnd = NULL;

  gbl_file_hnd = fopen(const_cast<char*>(filename.c_str()), "r");

  if(gbl_file_hnd == NULL || gbl_size == NULL) {
    return EMBER_BAD_ARGUMENT;
  }

  // Discard any previous image
  free_gbl_image();

  fseek(gbl_file_hnd, 0L, SEEK_END);
  *gbl_size = ftell(gbl_file_hnd);

  fseek(gbl_file_hnd, 0L, SEEK_SET);

  *gbl_image = (uint8_t*)malloc(*gbl_size);
  if(*gbl_image == NULL)
      return EMBER_ERR_FATAL;

  fread(*gbl_image, sizeof(uint8_t), *gbl_size, gbl_file_hnd);
  fclose(gbl_file_hnd);

  return EMBER_SUCCESS;
}
