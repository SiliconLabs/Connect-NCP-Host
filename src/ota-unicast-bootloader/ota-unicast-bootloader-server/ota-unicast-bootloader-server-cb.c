/***************************************************************************//**
 * @brief Weakly defined callbacks for ota unicast bootloader servers.
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

#include <connect/ember.h>
#include <connect/ota-unicast-bootloader-server.h>
#include <connect/callback_dispatcher.h>

//------------------------------------------------------------------------------
// Weak callbacks definitions

__attribute__ ((weak)) bool emberAfPluginOtaUnicastBootloaderServerGetImageSegmentCallback(
  uint32_t startIndex,
  uint32_t endIndex,
  uint8_t imageTag,
  uint8_t *imageSegment
  )
{
  (void)startIndex;
  (void)endIndex;
  (void)imageTag;
  (void)imageSegment;

  return false;
}

__attribute__ ((weak)) void emberAfPluginOtaUnicastBootloaderServerImageDistributionCompleteCallback(
  EmberAfOtaUnicastBootloaderStatus status)
{
  (void)status;
}

__attribute__ ((weak)) void emberAfPluginOtaUnicastBootloaderServerRequestTargetBootloadCompleteCallback(
  EmberAfOtaUnicastBootloaderStatus status)
{
  (void)status;
}
