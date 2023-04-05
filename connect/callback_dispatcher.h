#include "connect/ember.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __CALLBACK_DISPATCHER_H__
#define __CALLBACK_DISPATCHER_H__

// Init
void emberAfInit(void);

// Tick
void emberAfTick(void);

// Stack Status
void emberAfStackStatus(EmberStatus status);

// Child Join
void emberAfChildJoin(EmberNodeType nodeType,
                      EmberNodeId nodeId);

// Radio Needs Calibrating
void emberAfRadioNeedsCalibrating(void);

// Stack ISR
void emberAfStackIsr(void);

// Message Sent
void emberAfMessageSent(EmberStatus status,
                        EmberOutgoingMessage *message);

// Mac Message Sent
void emberAfMacMessageSent(EmberStatus status,
                           EmberOutgoingMacMessage *message);

// Incoming Message
void emberAfIncomingMessage(EmberIncomingMessage *message);

// Mac Incoming Message
void emberAfIncomingMacMessage(EmberIncomingMacMessage *message);

// Incoming Beacon
void emberAfIncomingBeacon(EmberPanId panId,
                           EmberMacAddress *source,
                           int8_t rssi,
                           bool permitJoining,
                           uint8_t beaconFieldsLength,
                           uint8_t *beaconFields,
                           uint8_t beaconPayloadLength,
                           uint8_t *beaconPayload);

// Active Scan Complete
void emberAfActiveScanComplete(void);

// Energy Scan Complete
void emberAfEnergyScanComplete(int8_t mean,
                               int8_t min,
                               int8_t max,
                               uint16_t variance);

// Mark Application Buffers
void emberAfMarkApplicationBuffers(void);

// Frequency Hopping Start Client Complete
void emberAfFrequencyHoppingStartClientComplete(EmberStatus status);

/**
 * @addtogroup app_framework_common
 * @brief Application framework common
 *
 * Declare all required application framework globals, initialize the Connect stack, and
 * dispatch stack callbacks calls as needed to the application components.
 *
 * @{
 *
 * @{
 * @name Callbacks
 */

/** @brief Application Framework Initialization Callback
 *
 * A callback invoked once during the initialization. It is called after the stack and plugins
 * initialization.
 *
 */
void emberAfInitCallback(void);

/** @brief Application Framework Tick Callback
 *
 * A callback invoked in each iteration of the application super loop and
 * can be used to perform periodic functions.  The frequency with which this
 * function is called depends on how quickly the main loop runs.  If the
 * application blocks at any time during the main loop, this function will not
 * be called until execution resumes.
 *
 */
void emberAfTickCallback(void);

/** @brief  Application framework equivalent of ::emberStackStatusHandler
 */
void emberAfStackStatusCallback(EmberStatus status);

/** @brief Application framework equivalent of ::emberIncomingMessageHandler
 */
void emberAfIncomingMessageCallback(EmberIncomingMessage *message);

/** @brief Application framework equivalent of ::emberIncomingMacMessageHandler
 */
void emberAfIncomingMacMessageCallback(EmberIncomingMacMessage *message);

/** @brief Application framework equivalent of ::emberMessageSentHandler
 */
void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message);

/** @brief Application framework equivalent of ::emberMacMessageSentHandler
 */
void emberAfMacMessageSentCallback(EmberStatus status,
                                   EmberOutgoingMacMessage *message);

/** @brief Application framework equivalent of ::emberChildJoinHandler
 *  @warning Requires the parent support plugin installed.
 */
void emberAfChildJoinCallback(EmberNodeType nodeType,
                              EmberNodeId nodeId);

/** @brief Application framework equivalent of ::emberActiveScanCompleteHandler
 */
void emberAfActiveScanCompleteCallback(void);

/** @brief Application framework equivalent of ::emberEnergyScanCompleteHandler
 */
void emberAfEnergyScanCompleteCallback(int8_t mean,
                                       int8_t min,
                                       int8_t max,
                                       uint16_t variance);

/** @brief Application framework equivalent of ::emberMarkApplicationBuffersHandler
 */
void emberAfMarkApplicationBuffersCallback(void);

/** @brief Application framework equivalent of ::emberIncomingBeaconHandler
 */
void emberAfIncomingBeaconCallback(EmberPanId panId,
                                   EmberMacAddress *source,
                                   int8_t rssi,
                                   bool permitJoining,
                                   uint8_t beaconFieldsLength,
                                   uint8_t *beaconFields,
                                   uint8_t beaconPayloadLength,
                                   uint8_t *beaconPayload);

/** @brief Application framework equivalent of ::emberFrequencyHoppingStartClientCompleteHandler
 */
void emberAfFrequencyHoppingStartClientCompleteCallback(EmberStatus status);

/** @brief Application framework equivalent of ::emberRadioNeedsCalibratingHandler
 */
void emberAfRadioNeedsCalibratingCallback(void);

/** @brief Application framework equivalent of ::emberStackIdleHandler
 */
bool emberAfStackIdleCallback(uint32_t *idleTimeMs);

/** @brief Application framework Low Power notification Callback
 *
 * A callback invoked when the system is about to go sleeping.
 *
 *  @param[in] enter_em2   \b true if the system is about to sleep or \b false to idle.
 *
 *  @param[in] duration_ms   Duration of the low power period. Time to the next event.
 *
 *  @return \b true if the application allows the system to go to sleep.
 */
bool emberAfCommonOkToEnterLowPowerCallback(bool enter_em2,
                                            uint32_t duration_ms);

void emAfPluginOtaUnicastBootloaderServerIncomingMessageCallback(
  EmberIncomingMessage *message
  );

void emAfPluginOtaUnicastBootloaderServerMessageSentCallback(
  EmberStatus status,
  EmberOutgoingMessage *message
  );
/**
 * @}
 *
 * @}
 */

#endif

#ifdef __cplusplus
}
#endif
