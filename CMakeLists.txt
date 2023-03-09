# Connect stack lib
cmake_minimum_required(VERSION  3.16.3)
include(CheckIncludeFile)
check_include_file(sl_cpc.h LIBCPC_FOUND)

add_library(connect-host-lib
            SHARED
            src/stack/framework/byte-utilities.c
            src/stack/framework/callback_dispatcher.c
            src/csp/csp-format.c
            src/csp/csp-command-app.c
            src/csp/csp-command-callbacks.c
            inc/lib-connect-ncp.h
            src/host-common/cpc-host.c
            src/host-common/ncp-host-common.c
            src/host-common/callback-queue.c
            src/host-common/lib-init.c)
            
target_include_directories(connect-host-lib PRIVATE
                            src/
                            ./)
                           
target_link_libraries(connect-host-lib PRIVATE cpc)

install (TARGETS connect-host-lib
    DESTINATION lib)

file(
	GLOB 
	headers
	stack/*.h
)

install(FILES ${headers}
  DESTINATION include/${CMAKE_PROJECT_NAME})