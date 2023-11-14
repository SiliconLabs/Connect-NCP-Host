# Connect Host Sink CLI App #

A simple Sink CLI application to communicate with a NCP using Silicon Labs' Connect protocol. It's the Host-NCP version of the SoC Sink sample app and it's able to form a network for SoC Sensor apps to receive sensor data.
The appication uses the public CLI https://github.com/daniele77/cli, refer to it for more information. It allows auto completion and command history. The CLI also has the possibility to open a Telnet server to control the host from a remote session (eg. for the tests suite). It is done here using Boost Asio driver on port 5000 of the device (can be changed in main.cpp in the application code). To connect to the CLI session, call ```telnet host_name port``` in a terminal on your remote device.

CPC daemon needs to be running (and connected to a secondary) before launching the app.

The application is also using the Boost Asio library. To install it, simply call ```sudo apt-get install libboost-all-dev``` in a terminal.

## Compile the app
Run the following commands

```
# Compile the app
cd connect-host-library/app
mkdir build
cd build
cmake ../
make

# Execute the app
./host_sink_app
```

## CLI

To use the CLI, you can find a transcript of the available commands below.
The commands are the same as the ones available in the Soc Sink sample app, to be able to form a network Sensors can join.
When writing an hex value in the CLI for a short address or nodeId, the formats "0xhex" "0Xhex" and "hex" are accepted.
For hex payloads and longer contents, only the "hex" format is accepted.

## CLI Commands

The following commands are available in the CLI:
```
help                                            Help message

exit                                            Quit the session

form                                            Forms a network on the NCP
<channel>                                       Valid channel number on the PHY

pjoin                                           Permit join for a given time.
<timeout>                                       Number of seconds allowed to join or 0xFF for unlimited time.

set_tx_power                                    Sets the antenna output power
<power>                                         Power in 0.1 dB steps. Actual value is an approximated by the system and may differ.

set_tx_options                                  Set transmission options.
<combined values>                               Security(+0x01) MAC Ack(+0x02) High Prio(+0x04)

remove_child                                    Remove a specific sensor child from the list.
<address mode>                                  Address modes: short=0x02, long=0x03
<short address>                                 16-bit address, e.g. remove_child 2 0x0001 {} . Can be 0 if long address used.
<long address>                                  64-bit address, e.g. remove_child 3 0 {123456789abcdef0} . Can be {} if short address used.

info                                            Print general stack info, MCU ID, Network state, Node ID, PAN ID, Channel ID, etc.

leave                                           Forget the current network and revert to EMBER_NO_NETWORK

data                                            Send a single Connect data frame to another node
<destination>                                   e.g. 0xC001
<data>                                          series of hex bytes, e.g. {1234}

set_channel                                     Set Connect channel
<channel>                                       Valid channel number on the PHY

start_energy_scan                               Scan the energy level on the given channel and with number of samples
<channel>                                       Valid channel number on the PHY
<sample count>                                  Number of samples

set_security_key                                Set security key.
<key>                                           16-byte hey security key e.g. {AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA}

unset_security_key                              Unset any security key in effect.

counter                                         Print out the passed stack counter
<counter number>                                Internal counter ID to show

reset_network                                   Resets the network on the NCP

bootloader_unicast_set_target                   Set the target node address OTA Unicast Image transmission
<target ID>                                     16-bit node ID of the node which should receive the image

bootloader_unicast_distribute                   Initiates the OTA Unicast Image transmission for the target and tag selected
<image size>                                    Length of the GBL image transmitted in bytes
<image tag>                                     8-bit identifier of the image transmitted. The client must agree in the tag to accept the image.

bootloader_unicast_request_bootload             Requests the target node to bootload the image transmitted and reboot the node running the updated image.
<delay>                                         The client is expected to schedule the firmware update after this many milliseconds
<image tag>                                     8-bit identifier of the image transmitted. The client must agree in the tag to bootload.

bootloader_set_tag                              Sets the 8-bit image identifier (tag) that will be associated with the stored image.
<image tag>                                     8-bit identifier of the image transmitted.

load_gbl_file                                   Loads the selected GBL file from disk to RAM for transmitting it later to the target ode.
<filename>                                      Name of the GBL file to load.
```