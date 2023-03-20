# Connect stack lib
cmake_minimum_required(VERSION  3.16.3)
project(connecthost VERSION 0.1.0 DESCRIPTION "Connect NCP Host Lib")

include(CheckCCompilerFlag)
check_c_compiler_flag(-Wall                  CC_HAVE_WALL)
check_c_compiler_flag(-Wno-stringop-overread CC_HAVE_WNO_STRINGOP_OVERREAD)
check_c_compiler_flag(-Wno-stringop-overflow CC_HAVE_WNO_STRINGOP_OVERFLOW)
check_c_compiler_flag(-funwind-tables        CC_HAVE_UNWIND_TABLES)
check_c_compiler_flag(-rdynamic              CC_HAVE_RDYNAMIC)

# Greatly improve warning messages. There is no reason to not enable that.
if(CC_HAVE_WALL)
    add_compile_options(-Wall)
endif()

# ... but it generate two irrelevant warnings
if(CC_HAVE_WNO_STRINGOP_OVERREAD)
    add_compile_options(-Wno-stringop-overread)
endif()
if(CC_HAVE_WNO_STRINGOP_OVERFLOW)
    add_compile_options(-Wno-stringop-overflow)
endif()

# In come case, backtraces are not available without that flags. Increase size
# of the  binary file, but can be stripped.
if(CC_HAVE_UNWIND_TABLES)
    add_compile_options(-funwind-tables)
endif()

if(CC_HAVE_RDYNAMIC)
    add_link_options(-rdynamic)
endif()


include(CheckIncludeFile)
check_include_file(sl_cpc.h LIBCPC_FOUND)

add_definitions(-DUNIX_HOST
                -DSL_CATALOG_CONNECT_AES_SECURITY_PRESENT
                )
                
add_library(connecthost
            SHARED
            src/stack/framework/byte-utilities.c
            src/stack/framework/callback_dispatcher.c
            src/csp/csp-format.c
            src/csp/csp-command-app.c
            src/csp/csp-command-callbacks.c
            src/host-common/cpc-host.c
            src/host-common/ncp-host-common.c
            src/host-common/callback-queue.c
            src/host-common/lib-init.c
            src/log/log.c
            src/log/backtrace_show.c)

set_target_properties(connecthost PROPERTIES VERSION ${PROJECT_VERSION})
            
target_include_directories(connecthost 
                            PRIVATE
                            src/
                            ./)

set_property(TARGET connecthost PROPERTY
            PUBLIC_HEADER
            connect/ncp.h
            connect/ember.h
            connect/byte-utilities.h
            connect/callback_dispatcher.h
            connect/crypto-types.h
            connect/ember-types.h
            connect/error-def.h
            connect/error.h
            connect/message.h
            connect/version.h
            connect/network-management.h
            connect/radio-stream.h
            connect/stack-info.h
            connect/version.h)

target_link_libraries(connecthost PRIVATE cpc)

configure_file(connecthost.pc.in connecthost.pc @ONLY)

include(GNUInstallDirs)
install (TARGETS connecthost
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/connect)
install(FILES ${CMAKE_BINARY_DIR}/connecthost.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)