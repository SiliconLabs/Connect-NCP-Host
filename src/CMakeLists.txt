# Connect stack lib
include(CheckIncludeFile)
check_include_file(sl_cpc.h LIBCPC_FOUND)

add_library(connect-host-lib
            SHARED
            stack/framework/byte-utilities.c
            stack/framework/callback_dispatcher.c
            csp/csp-format.c
            csp/csp-command-app.c
            csp/csp-command-callbacks.c
            include/lib-connect-ncp.h
            host-common/cpc-host.c
            host-common/ncp-host-common.c
            host-common/callback-queue.c
            host-common/lib-init.c)
            
target_include_directories(connect-host-lib PUBLIC
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