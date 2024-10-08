Connect NCP Host Library Release Note
==============================

# Release 2.0
(release date 2024-10-08)

## New Features and Improvements
* added support of SUN-FSK and SUN-OFDM. Enable the transmission and reception of packets carrying up to 2033 bytes of payload.
* added APIs emberOfdmSetMcs() and emberOfdmGetMcs() to get and set the OFDM MCS when SUN-OFDM is used.
* update EmberMessageLength typedef to allow the message length to be coded on 16 bits.
* added API emberUsingLongMessages() to enable the support of NCP messages codes on 16 bits. Default to 8 bits messages.
* added API emberSetUnencryptedPacketsAcceptance() to accept or refuse unencrypted packets.

## Bug Fixes
* fixed a segmentation fault while freeing a new firmware image

# Release 1.1.0
(release date 2023-10-17)

## New Features and Improvements
* added API emberGetVersionInfo() to get GSDK, Stack and bootloader versions
* added API sl_connect_get_ncp_gsdk_version() to get the GSDK version on the NCP without calling the stack
* added APIs emberGetKeyId() and emberSetPsaSecurityKey() to manage PSA security keys by their ID
* added API emberSetNcpSecurityKeyPersistent() to set a permanent PSA security key
* changed the way the NCP connection is opened through CPC

# Release 1.0.0
(release date 2023-06-09)

## New Features and Improvements
* added OTA Unicast bootloader server support
* added example application

# Release 0.1.0
(release date 2023-03-15)

## New Features and Improvements
* added CPC support for communication with the NCP
* added public APIs for linux host application development