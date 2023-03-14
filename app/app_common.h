/***************************************************************************//**
 * @file
 * @brief sl_app_common.h
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __APP_COMMON__
#define __APP_COMMON__

// -----------------------------------------------------------------------------
//                                   Includes
// -----------------------------------------------------------------------------
#include "stack/include/ember.h"
#include "sl_sensor_sink_config.h"

// -----------------------------------------------------------------------------
//                              Macros and Typedefs
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//                                Global Variables
// -----------------------------------------------------------------------------

/// Connect security key set up by CLI (or the default key)
extern EmberKeyData security_key;
/// TX options set up for the network
extern EmberMessageOptions tx_options;

// -----------------------------------------------------------------------------
//                          Public Function Declarations
// -----------------------------------------------------------------------------
#endif // __APP_COMMON__

extern void printBuffer(const uint8_t* buffer, uint8_t length);

#ifdef __cplusplus
}
#endif