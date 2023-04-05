#include "connect/callback_dispatcher.h"

__attribute__ ((weak)) void emberAfInit(void)
{
}

__attribute__ ((weak)) void emberAfTick(void)
{
}

__attribute__ ((weak)) void emberAfStackStatus(EmberStatus status)
{
  (void)status;
}

__attribute__ ((weak)) void emberAfChildJoin(EmberNodeType nodeType,
                                             EmberNodeId nodeId)
{
  (void)nodeType;
  (void)nodeId;
}

__attribute__ ((weak)) void emberAfRadioNeedsCalibrating(void)
{
}

__attribute__ ((weak)) void emberAfStackIsr(void)
{
}

__attribute__ ((weak)) void emberAfMessageSent(EmberStatus status,
                                               EmberOutgoingMessage *message)
{
  emAfPluginOtaUnicastBootloaderServerMessageSentCallback(
    status,
    message
    );
}

__attribute__ ((weak)) void emberAfMacMessageSent(EmberStatus status,
                                                  EmberOutgoingMacMessage *message)
{
  (void)status;
  (void)message;
}

__attribute__ ((weak)) void emberAfIncomingMessage(EmberIncomingMessage *message)
{
  emAfPluginOtaUnicastBootloaderServerIncomingMessageCallback(message);
}

__attribute__ ((weak)) void emberAfIncomingMacMessage(EmberIncomingMacMessage *message)
{
  (void)message;
}

__attribute__ ((weak)) void emberAfIncomingBeacon(EmberPanId panId,
                                                  EmberMacAddress *source,
                                                  int8_t rssi,
                                                  bool permitJoining,
                                                  uint8_t beaconFieldsLength,
                                                  uint8_t *beaconFields,
                                                  uint8_t beaconPayloadLength,
                                                  uint8_t *beaconPayload)
{
  (void)panId;
  (void)source;
  (void)rssi;
  (void)permitJoining;
  (void)beaconFieldsLength;
  (void)beaconFields;
  (void)beaconPayloadLength;
  (void)beaconPayload;
}

__attribute__ ((weak)) void emberAfActiveScanComplete(void)
{
}

__attribute__ ((weak)) void emberAfEnergyScanComplete(int8_t mean,
                                                      int8_t min,
                                                      int8_t max,
                                                      uint16_t variance)
{
  (void)mean;
  (void)min;
  (void)max;
  (void)variance;
}

__attribute__ ((weak)) void emberAfMarkApplicationBuffers(void)
{
}

__attribute__ ((weak)) void emberAfFrequencyHoppingStartClientComplete(EmberStatus status)
{
  (void)status;
}

//------------------------------------------------------------------------------
// Application callbacks weak definitions

__attribute__ ((weak)) void emberAfInitCallback(void)
{
}

__attribute__ ((weak)) void emberAfTickCallback(void)
{
}

__attribute__ ((weak)) void emberAfStackStatusCallback(EmberStatus status)
{
  (void)status;
}

__attribute__ ((weak)) void emberAfIncomingMessageCallback(EmberIncomingMessage *message)
{
  (void)message;
}

__attribute__ ((weak)) void emberAfIncomingMacMessageCallback(EmberIncomingMacMessage *message)
{
  (void)message;
}

__attribute__ ((weak)) void emberAfMessageSentCallback(EmberStatus status,
                                                       EmberOutgoingMessage *message)
{
  (void)status;
  (void)message;
}

__attribute__ ((weak)) void emberAfMacMessageSentCallback(EmberStatus status,
                                                          EmberOutgoingMacMessage *message)
{
  (void)status;
  (void)message;
}

__attribute__ ((weak)) void emberAfChildJoinCallback(EmberNodeType nodeType,
                                                     EmberNodeId nodeId)
{
  (void)nodeType;
  (void)nodeId;
}

__attribute__ ((weak)) void emberAfActiveScanCompleteCallback(void)
{
}

__attribute__ ((weak)) void emberAfEnergyScanCompleteCallback(int8_t mean,
                                                              int8_t min,
                                                              int8_t max,
                                                              uint16_t variance)
{
  (void)mean;
  (void)min;
  (void)max;
  (void)variance;
}

__attribute__ ((weak)) void emberAfMarkApplicationBuffersCallback(void)
{
}

__attribute__ ((weak)) void emberAfIncomingBeaconCallback(EmberPanId panId,
                                                          EmberMacAddress *source,
                                                          int8_t rssi,
                                                          bool permitJoining,
                                                          uint8_t beaconFieldsLength,
                                                          uint8_t *beaconFields,
                                                          uint8_t beaconPayloadLength,
                                                          uint8_t *beaconPayload)
{
  (void)panId;
  (void)source;
  (void)rssi;
  (void)permitJoining;
  (void)beaconFieldsLength;
  (void)beaconFields;
  (void)beaconPayloadLength;
  (void)beaconPayload;
}

__attribute__ ((weak)) void emberAfFrequencyHoppingStartClientCompleteCallback(EmberStatus status)
{
  (void)status;
}

__attribute__ ((weak)) void emberAfRadioNeedsCalibratingCallback(void)
{
}

__attribute__ ((weak)) bool emberAfStackIdleCallback(uint32_t *idleTimeMs)
{
  (void)idleTimeMs;

  return false;
}
