/***************************************************************************//**
 * @file
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

/**
 * @addtogroup message
 * @brief Connect APIs and handlers for sending and receiving messages.
 *
 * Note that MAC mode and Extended star/direct mode use different APIs for
 * messaging.
 *
 * See message.h for source code.
 * @{
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/** @brief \deprecated  The maximum length in bytes of the application payload
 *  for an unsecured message.  This define has been deprecated, you should use
 *  the ::emberGetMaximumPayloadLength API instead.
 */
#define EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH     111

/** @brief \deprecated  The maximum length in bytes of the application payload
 * for a secured message. This define has been deprecated, you should use the
 * ::emberGetMaximumPayloadLength API instead.
 */
#define EMBER_MAX_SECURED_APPLICATION_PAYLOAD_LENGTH       102

/** @brief The maximum allowed endpoint value.
 */
#define EMBER_MAX_ENDPOINT                                 0xF

/** @brief Send a message to the passed destination short ID.
 *
 * @param[in] destination The destination node short ID.
 *
 * @param[in] endpoint    The destination endpoint of the outgoing message.
 *                        This value can't exceed ::EMBER_MAX_ENDPOINT.
 *
 * @param[in] messageTag  A value chosen by the application. This value will be
 *                        passed in the corresponding
 *                        ::emberMessageSentHandler() call.
 *
 * @param[in] messageLength The size of the message payload in bytes. Use the
 * ::emberGetMaximumPayloadLength() API to determine the maximum message length
 * allowed.
 *
 * @param[in] message A pointer to an array of bytes containing the message
 * payload.
 *
 * @param[in] options  Specifies the ::EmberMessageOptions for the outgoing
 * message.
 *
 * @return an ::EmberStatus value of:
 * - ::EMBER_SUCCESS if the message was accepted by the stack. If a success
 * status is returned, the ::emberMessageSentHandler() callback is invoked
 * by the stack to indicate whether the message was successfully delivered or
 * the reason for failure.
 * - ::EMBER_INVALID_CALL if the node is not joined to a network or the node is
 * of ::EMBER_MAC_MODE_DEVICE device type or ::EMBER_MAC_MODE_SLEEPY_DEVICE (use
 * ::emberMacMessageSend instead).
 * - ::EMBER_BAD_ARGUMENT if the packet length is 0, the passed TX options
 * indicates some feature that is not supported, the passed endpoint
 * exceeds ::EMBER_MAX_ENDPOINT
 * - ::EMBER_MESSAGE_TOO_LONG if the message does not fit in a single frame.
 * - ::EMBER_PHY_TX_BUSY if the message cannot be sent since the node does not
 * support MAC queuing and the radio is currently busy.
 * - ::EMBER_MAC_TRANSMIT_QUEUE_FULL if the outgoing MAC queue is currently
 * full.
 * - ::EMBER_NO_BUFFERS if the stack could not allocate enough RAM to store the
 * submitted message.
 * - ::EMBER_MAC_UNKNOWN_DESTINATION if the node is part of a star network and
 * the destination node does not appear in the node's routing table.
 * - ::EMBER_MAC_SECURITY_NOT_SUPPORTED if the message was requested to be sent
 * out secured and either the local node does not support security or the
 * destination node is known to not support security.
 * - ::EMBER_MAC_BUSY if the message was not accepted because the MAC is
 * currently performing some critical operation.
 */
EmberStatus emberMessageSend(EmberNodeId destination,
                             uint8_t endpoint,
                             uint8_t messageTag,
                             EmberMessageLength messageLength,
                             uint8_t *message,
                             EmberMessageOptions options);

/** @brief Create a MAC level frame and sends it to the passed destination.
 * This API can only be used for nodes of ::EMBER_MAC_MODE_DEVICE node type or
 * ::EMBER_MAC_MODE_SLEEPY_DEVICE node type.
 *
 * @param[in] macFrame   A pointer to an ::EmberMacFrame struct that specifies
 * the source and destination addresses and the source and destination PAN IDs
 * for the message to be sent. Note that if the source/destination PAN ID is not
 * specified, it defaults to the node's PAN ID. Also, the destination
 * address mode must be either ::EMBER_MAC_ADDRESS_MODE_SHORT or
 * ::EMBER_MAC_ADDRESS_MODE_LONG.
 *
 * @param[in] messageTag  A value chosen by the application. This value will be
 * passed in the corresponding ::emberMacMessageSentHandler() call.
 *
 * @param[in] messageLength The size in bytes of the message payload. The
 * application can use the ::emberGetMaximumPayloadLength() API to determine the
 * maximum allowable payload, given a permutation of source and destination
 * addressing and other TX options.
 *
 * @param[in] message A pointer to an array of bytes containing the message
 * payload.
 *
 * @param[in] options Specifies the ::EmberMessageOptions for the outgoing
 * message.
 *
 * @return an ::EmberStatus value of:
 * - ::EMBER_SUCCESS if the message was accepted by the stack. If a success
 * status is returned, the ::emberMacMessageSentHandler() callback will be
 * invoked by the stack to indicate whether the message was successfully
 * delivered or the reason for failure.
 * - ::EMBER_INVALID_CALL if the node is of a node type other than
 * ::EMBER_MAC_MODE_DEVICE or ::EMBER_MAC_MODE_SLEEPY_DEVICE.
 * - ::EMBER_BAD_ARGUMENT if the packet length is 0, the passed TX options
 * indicates some feature that is not supported or the destination address mode
 * is set to ::EMBER_MAC_ADDRESS_MODE_NONE.
 * - ::EMBER_MESSAGE_TOO_LONG if the message does not fit in a single frame.
 * - ::EMBER_PHY_TX_BUSY if the message cannot be sent since the node does not
 * support MAC queuing and the radio is currently busy.
 * - ::EMBER_MAC_TRANSMIT_QUEUE_FULL if the outgoing MAC queue is currently
 * full.
 * - ::EMBER_NO_BUFFERS if the stack could not allocate enough RAM to store the
 * submitted message.
 * - ::EMBER_MAC_SECURITY_NOT_SUPPORTED if the message was requested to be sent
 * out with a security but no security plugin was enabled.
 * - ::EMBER_MAC_BUSY if the message was not accepted because the MAC is
 * currently performing a critical operation.
 */
EmberStatus emberMacMessageSend(EmberMacFrame *macFrame,
                                uint8_t messageTag,
                                EmberMessageLength messageLength,
                                uint8_t *message,
                                EmberMessageOptions options);

/** @brief Send a data request command to the parent node. Note that if the
 * node short ID is a value of ::EMBER_USE_LONG_ADDRESS, the node shall use its
 * long ID as source address.
 *
 * @return and ::EmberStatus value of:
 * - ::EMBER_SUCCESS if the data poll was accepted by the MAC layer.
 * - ::EMBER_INVALID_CALL if the node is not joined to a network, the node is
 * not an end device, an ::EMBER_MAC_MODE_DEVICE or an
 * ::EMBER_MAC_MODE_SLEEPY_DEVICE, or the node is of ::EMBER_MAC_MODE_DEVICE
 * or ::EMBER_MAC_MODE_SLEEPY_DEVICE node type, is not joined to a coordinator
 * and the poll destination was not correctly set via the
 * ::emberSetPollDestinationAddress() API.
 * - ::EMBER_MAC_BUSY if the MAC is currently performing a critical
 * operation.
 */
EmberStatus emberPollForData(void);

/** @brief Set data polls destination address for nodes of
 * ::EMBER_MAC_MODE_DEVICE node type or ::EMBER_MAC_MODE_SLEEPY_DEVICE node
 *  type.
 *
 * @return and ::EmberStatus value of ::EMBER_SUCCESS if the data poll
 * destination was correctly set, or another ::EmberStatus value indicating the
 * reason of failure.
 */
EmberStatus emberSetPollDestinationAddress(EmberMacAddress *destination);

/**
 * @brief Purge all indirect transmissions from the indirect message queue.
 *
 * @return an ::EmberStatus value of ::EMBER_SUCCESS if all indirect
 * messages were purged, or another ::EmberStatus value indicating the reason
 * of failure.
 *
 * @ingroup parent_support
 **/
EmberStatus emberPurgeIndirectMessages(void);

/**
 * @brief Set indirect queue timeout value. The indirect queue timeout
 * is set by default to ::EMBER_INDIRECT_TRANSMISSION_TIMEOUT_MS.
 *
 * @param timeoutMs  The timeout in milliseconds to be set.
 *
 * @return an ::EmberStatus value of ::EMBER_SUCCESS if the passed timeout was
 * successfully set, or a value of ::EMBER_BAD_ARGUMENT if the passed value is
 * invalid.
 *
 * @ingroup parent_support
 **/
EmberStatus emberSetIndirectQueueTimeout(uint32_t timeoutMs);

/**
 * @brief Return the maximum payload according to the passed source and
 * destination addressing modes, the passed secured flag, and the current
 * configuration of the node.
 *
 * @param[in] srcAddressMode  An ::EmberMacAddressMode value indicating the mode
 * of the source address. Note, this parameter is only meaningful if the node
 * was started as ::EMBER_MAC_MODE_DEVICE or ::EMBER_MAC_MODE_SLEEPY_DEVICE.
 *
 * @param[in] dstAddressMode  An ::EmberMacAddressMode value indicating the mode
 * of the destination address. Note, this parameter is only meaningful if the
 * node was started as ::EMBER_MAC_MODE_DEVICE or
 * ::EMBER_MAC_MODE_SLEEPY_DEVICE.
 *
 * @param[in] interpan  Indicates whether the frame is an interpan frame or not.
 * Note, this parameter is only meaningful if the node was started as
 * ::EMBER_MAC_MODE_DEVICE or ::EMBER_MAC_MODE_SLEEPY_DEVICE.
 *
 * @param[in] secured   Indicates whether the frame should be secured or not.
 *
 * @return The maximum payload length in bytes achievable according to the
 * passed parameters or \b 0xFF if the node is currently active on a network or
 * any of the passed parameters are invalid.
 **/
uint16_t emberGetMaximumPayloadLength(EmberMacAddressMode srcAddressMode,
                                      EmberMacAddressMode dstAddressMode,
                                      bool interpan,
                                      bool secured);

/**
 * @brief Indicates if the stack is currently using long messages or not.
 *
 * @return True if the stack currently uses long messages (length stored in a
 * uint16_t) or false if it is not the case (length stored in a uint8_t).
 *
 **/
bool emberUsingLongMessages(void);

/**
 * @brief Set the current message length that the stack uses.
 *
 * @param useLongMessages True to use long messages (length stored in a
 * uint16_t), false to use short messages (length stored in a uint8_t).
 *
 * @note This API is here to assure retro compatibility with old NCP Host lib versions.
 * In NCP Host lib versions that do not support OFDM or SUN-FSK features
 * (v1.1 and older), only short messaging is supported.
 * For the NCP, short messages are used by default. The Host application needs to call
 * this API with useLongMessages set to true to use OFDM or SUN-FSK.
 *
 * @warning This API changes the behavior of the Connect Serialization Protocol.
 * It only has effect when using a RTOS or the NCP. Changing it may result in
 * packets being incorrectly transfered through CSP when using a SUN-OFDM or
 * SUN-FSK PHY.
 **/
EmberStatus emberNcpSetLongMessagesUse(bool useLongMessages);

#endif //__MESSAGE_H__

/** @} // END addtogroup
 */
