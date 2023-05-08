/***************************************************************************//**
 * @file
 * @brief 
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

#ifndef __CLI_HANDLERS_H__
#define __CLI_HANDLERS_H__

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------

#include <cli/cli.h>

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------


// CLI command handlers
void cli_form(std::ostream&,
              uint16_t radioChannel);

void cli_pjoin(std::ostream&, std::string duration);

void cli_set_tx_power(std::ostream&,
                      int16_t power);

void cli_set_tx_options(std::ostream&, std::string i_sendOptions);

void cli_remove_child(std::ostream&,
                          uint8_t mode,
                          std::string shortAddressHex,
                          std::string longAdressHex);

void cli_info(std::ostream& out);

void cli_leave(std::ostream&);

void cli_data(std::ostream&, std::string destination, std::string payload);

void cli_set_channel(std::ostream&, uint16_t radioChannel);

void cli_start_energy_scan(std::ostream&,
                           uint16_t channelToScan,
                           uint8_t samples);

void cli_set_security_key(std::ostream&, std::string keyContents);

void cli_unset_security_key(std::ostream&);

void cli_counter(std::ostream&, uint8_t counterType);

void reset_network_command(std::ostream&);

void cli_bootloader_unicast_set_target(std::ostream&,
                                       std::string target_id_hex);

void cli_bootloader_unicast_distribute(std::ostream&,
                                       uint32_t image_size,
                                       std::string image_tag_hex);

void cli_bootloader_unicast_request_bootload(std::ostream&,
                                             uint32_t delay_ms,
                                             std::string image_tag_hex);

void cli_bootloader_set_tag(std::ostream&,
                            std::string new_image_tag_hex);

void cli_load_gbl_file(std::ostream&,
                       std::string filename);

#endif //__CLI_HANDLERS_H__
