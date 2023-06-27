/***************************************************************************//**
 * @brief CLI handlers for Connect NCP host app
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

#include <unistd.h>
#include <stdio.h>
#include <cli/cli.h>
#include "app_cli.h"

void load_cli_commands(std::unique_ptr<cli::Menu>& rootMenu)
{
  rootMenu->Insert(
    "form",
    cli_form,
    "Forms a network on the NCP\n \
       <channel>:         Valid channel number on the PHY");
  rootMenu->Insert(
    "pjoin",
    cli_pjoin,
    "Permit join for a given time period\n \
       <timeout>:         Number of seconds allowed to join or 0xFF for unlimited time");
  rootMenu->Insert(
    "set_tx_power",
    cli_set_tx_power,
    "Sets the antenna output power\n \
       <power>:           Power in 0.1 dB steps. Actual value is an approximated by the system and may differ");
  rootMenu->Insert(
    "set_tx_options",
    cli_set_tx_options,
    "Set transmission options\n \
       <combined values>: Security(+0x01) MAC Ack(+0x02) High Prio(+0x04)");
  rootMenu->Insert(
    "remove_child",
    cli_remove_child,
    "Remove a specific sensor child from the list.\n \
       <address mode>:    Address modes: short=0x02, long=0x03\n \
       <short address>:   16-bit address, e.g. remove_child 2 0x0001 {} .Can be 0 if long address used\n \
       <long address>:    64-bit address, e.g. remove_child 3 0 {123456789abcdef0} . Can be {} if short address used");
  rootMenu->Insert(
    "info",
    cli_info,
    "Print general stack info, MCU ID, Network state, Node ID, PAN ID, Channel ID, etc.");
  rootMenu->Insert(
    "leave",
    cli_leave,
    "Forget the current network and revert to EMBER_NO_NETWORK");
  rootMenu->Insert(
    "data",
    cli_data,
    "Send a single Connect data frame to another node\n \
       <destination>:     e.g. 0xC001\n \
       <data>:            series of hex bytes, e.g. {1234}");
  rootMenu->Insert(
    "set_channel",
    cli_set_channel,
    "Set Connect channel\n \
       <channel>:         Valid channel number on the PHY");
  rootMenu->Insert(
    "start_energy_scan",
    cli_start_energy_scan,
    "Scan the energy level on the given channel and with number of samples\n \
       <channel>:         Valid channel number on the PHY\n \
       <sample count>:    Number of samples");
  rootMenu->Insert(
    "set_security_key",
    cli_set_security_key,
    "Set security key. \n \
       <key>:             16-byte hey security key e.g. {AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA}");
  rootMenu->Insert(
    "unset_security_key",
    cli_unset_security_key,
    "Unset any security key in effect");
  rootMenu->Insert(
    "counter",
    cli_counter,
    "Print out the passed stack counter\n \
       <counter number>:  Internal counter ID to show");
  rootMenu->Insert(
    "reset_network",
    reset_network_command,
    "Resets the network on the NCP");
  rootMenu->Insert(
    "bootloader_unicast_set_target",
    cli_bootloader_unicast_set_target,
    "Set the target node address OTA Unicast Image transmission\n \
       <target ID>        16-bit node ID of the node which should receive the image");
  rootMenu->Insert(
    "bootloader_unicast_distribute",
    cli_bootloader_unicast_distribute,
    "Initiates the OTA Unicast Image transmission for the target and tag selected\n \
       <image size>       Length of the GBL image transmitted in bytes\n \
       <image tag>        8-bit identifier of the image transmitted. The client must agree in the tag to accept the image");
  rootMenu->Insert(
    "bootloader_unicast_request_bootload",
    cli_bootloader_unicast_request_bootload,
    "Requests the target node to bootload the image transmitted and reboot the node running the updated image\n \
       <delay>            The client is expected to schedule the firmware update after this many milliseconds\n \
       <image tag>        8-bit identifier of the image transmitted. The client must agree in the tag to bootload");
  rootMenu->Insert(
    "bootloader_set_tag",
    cli_bootloader_set_tag,
    "Sets the 8-bit image identifier (tag) that will be associated with the stored image\n \
       <image tag>        8-bit identifier of the image transmitted");
  rootMenu->Insert(
    "load_gbl_file",
    cli_load_gbl_file,
    "Loads the selected GBL file from disk to RAM for transmitting it later to the target node\n \
       <filename>         Name of the GBL file to load");
}
