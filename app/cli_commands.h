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

#ifndef __CLI_COMMANDS_H__
#define __CLI_COMMANDS_H__

#include <cli/cli.h>
#include <unistd.h>
#include <stdio.h>

void load_cli_commands(std::unique_ptr<cli::Menu>& rootMenu);

#endif
