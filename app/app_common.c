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
// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <connect/ember-types.h>
#include "sl_sensor_sink_config.h"

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

/// GBL image storage
uint8_t* gbl_image = NULL;

/// Connect security key (default)
EmberKeyData default_security_key = { .contents = SL_SENSOR_SINK_SECURITY_KEY };

// -----------------------------------------------------------------------------
//                          Public Function Definitions
// -----------------------------------------------------------------------------

/******************************************************************************
* Prints a byte array as a string of hex values
******************************************************************************/
void printBuffer(const uint8_t* buffer, uint8_t length)
{
  uint8_t i;
  for (i = 0; i < length; i++) {
    printf("%02X", buffer[i]);
  }
}

/******************************************************************************
* Helper to free the memory buffer of a GBL file no longer used
******************************************************************************/
void free_gbl_image(void)
{
  if(gbl_image != NULL) {
    free(gbl_image);
    gbl_image = NULL;
  }
}
