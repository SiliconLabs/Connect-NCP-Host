cmake_minimum_required(VERSION  3.16.3)
project(connect_host_sink_app)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

include(CheckCCompilerFlag)
check_c_compiler_flag(-Wall                  CC_HAVE_WALL)
check_c_compiler_flag(-Wno-stringop-overread CC_HAVE_WNO_STRINGOP_OVERREAD)
check_c_compiler_flag(-Wno-stringop-overflow CC_HAVE_WNO_STRINGOP_OVERFLOW)



# find_package(MbedTLS REQUIRED)
# mark_as_advanced(MbedTLS_DIR)

# Compile options
if(CC_HAVE_WALL)
    add_compile_options(-Wall)
endif()
if(CC_HAVE_WNO_STRINGOP_OVERREAD)
    add_compile_options(-Wno-stringop-overread)
endif()
if(CC_HAVE_WNO_STRINGOP_OVERFLOW)
    add_compile_options(-Wno-stringop-overflow)
endif()

add_definitions(-DUNIX_HOST
                # -DSL_CATALOG_CONNECT_AES_SECURITY_PRESENT
                )

# Source files and executable
include(FetchContent)
FetchContent_Declare(
  cli
  GIT_REPOSITORY https://github.com/daniele77/cli.git
  GIT_TAG v2.0.2
)
FetchContent_MakeAvailable(cli)

add_subdirectory(src/)

add_executable(host_sink_app 
               app/app_init.c
               app/app_process.c
               app/app_cli.cpp
               app/app_common.c
               app/cli_commands.cpp
               app/main.cpp
               )


target_include_directories(host_sink_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    app/
    app/config
    )

# External libraries
target_link_libraries(host_sink_app PUBLIC connect-host-lib)
target_link_libraries(host_sink_app PRIVATE cli::cli)
