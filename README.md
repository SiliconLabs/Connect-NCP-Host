# Connect NCP Host Library #


## Dependencies

The library is delivered as a CMake project. The minimum version of CMake required to generate the project is 3.16.

Silicon Labs' CPC daemon and library are necessary for the Connect NCP Host library to compile. You can get them here: https://github.com/SiliconLabs/cpc-daemon. Run the following commands to install it:
```
git clone https://github.com/SiliconLabs/cpc_daemon.git
cd cpc_daemon
mkdir build
cd build
cmake ../
make

# Install CPCD and CPC library
sudo make install
sudo ldconfig
```

For more information on how to run the daemon, refer to its documentation on GitHub or in the daemon's README.
## Build and use the library

The library is delivered through source files and a CMakelists.txt file that builds the connect-host-lib. To build the library and install it, it can be done by running in a terminal the following commands in this folder: 

``` 
mkdir build
cd build
cmake ../
make

sudo make install
sudo ldconfig
```

Then, in your application CMakelists simply add 

```
target_link_libraries(your_app_executable PUBLIC connect-host-lib)
```

The Connect API contains every function callable by the host application. It allows every functionality that is available on the SoC. The user can see what is available to him by consulting the connect-api.h header located at the root of the library.

#### sl_connect_ncp_init

The host needs to connect to the CPC daemon first before doing anything. This can only be done if the secondary is already connected to the daemon. The secondary is blocked until it detects the presence of the host to prevent having the stack run without the host. This is done by sending a first dummy message to the secondary once the host is connected.

The init function starts the poll thread and initialize the CPC communication.

#### sl_connect_poll_ncp_msg

This API needs to have its own dedicated polling thread, implemented by the user, to prevent blocking the communication with the NCP. This polling function detects CPC daemon's notifications through its associated file descriptor. Depending on the file descriptor's event, a confirmation or an indication is send to the application through a callback. Thus the thread only watches the CPC file descriptor and serves as a listener.

- In the event of a confirmation, the poll thread is used to unlock the application command mutex. This mutex is there to protect access to the CPC file descriptor to keep the Connect NCP Host Library thread safe and to serve as a synchronization barrier for the application.
- If an indication comes through CPC, the poll thread will forward the received command into a buffer that needs to be emptied by calling the sl_connect_ncp_handle_pending_callback_commands() function.
  
#### sl_connect_ncp_handle_pending_callback_commands

When an indication comes from the NCP, it is picked up by the poll thread and the command buffer is stored. In whichever application thread, this indication is processed through CSP to de-serialize the command, get the command ID and execute the corresponding application callback.

It is the application responsiblity to call sl_connect_ncp_handle_pending_callback_commands() to empty the queue buffer. It must not be called in the poll thread to prevent interlocking and blocking the API. In another thread or in the main application, the callback queue can be polled using sl_connect_ncp_poll_callback_command(timeout) in order to prevent blocking the queue and emptying the queue when a callback is in it and as soon as possible. This poll function just calls sl_connect_ncp_handle_pending_callback_commands().

The callback queue is a simple POSIX pipe. When appending a callback command into the pipe, its length is added to it to be able to parse it when calling sl_connect_ncp_handle_pending_callback_commands(). Indeed, multiple commands can be stored in the pipe but they are read all at once from the pipe output. For each read callback command, sli_connect_ncp_handle_indication() is called to execute the corresponding code.

### Includes and callbacks

Most of the library can be included with 
```
#include <connect/ncp.h>
```
This header contains public Connect stack types and APIs as well as the Host specific APIs that are presented above. For more specific includes, other headers are available in the folder *connect/*.

The library contains callbacks that can be implemented by the user. In the library, they are defined using a *weak* attribute and do nothing. To use user defined stack callbacks, just add the following include in the file(s) where you implement them:
```
#include <connect/callback_dispatcher.h>
```
The complete list of implementable callbacks can be found in this header.