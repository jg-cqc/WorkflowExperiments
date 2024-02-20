cmake_minimum_required(VERSION 3.20)

##################################
# Project Settings
##################################

project(qo_onboard_examples)

include(GNUInstallDirs)


##################################
# Compiler Settings
##################################

add_compile_definitions(ONBOARD_VERSION=${ONBOARD_VERSION})
add_compile_definitions(ONBOARD_VERSION_MAJOR=${ONBOARD_VERSION_MAJOR})
add_compile_definitions(ONBOARD_VERSION_MINOR=${ONBOARD_VERSION_MINOR})
add_compile_definitions(ONBOARD_VERSION_PATCH=${ONBOARD_VERSION_PATCH})

if(MSVC)
    #\include\cppcodec\detail\stream_codec.hpp(66): warning C4702: unreachable code
    # Disabled with flag /wd4702
    add_compile_options(/WX /W4 /wd4702)
else(MSVC)
    add_compile_options(-Wall -Werror -Werror=sign-conversion)
    # Disable warnings which trigger a compiler bug in GCC-12 when compiling on RedHat
    # See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105651
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION MATCHES "^12.*")
        add_compile_options(-Wno-error=restrict)
    endif()
endif(MSVC)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86(_64)?")
    add_compile_options(-mrdseed)
endif ()

if (PROFILE)
    add_compile_options(-g -pg)
endif ()

add_compile_definitions(INCLUDE_EXTRACTOR)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/O2)
    else(MSVC)
        add_compile_options(-O3)
    endif(MSVC)
endif ()

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


##################################
# Progress messages
##################################

message(STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "HOST SYSTEM PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "SYSTEM PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "C FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CXX FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "BUILD TYPE: ${CMAKE_BUILD_TYPE}")


########################################
# qo_onboard_sample_code_A_using_config_file
########################################

add_executable(qo_onboard_sample_code_A_using_config_file
               qo_onboard_sample_code_A_using_config_file.c)

target_link_libraries(qo_onboard_sample_code_A_using_config_file
                      spdlog::spdlog
                      rd_seed::rd_seed
                      qo_onboard_c)

########################################
# qo_onboard_sample_code_B_using_setopt_api
########################################

add_executable(qo_onboard_sample_code_B_using_setopt_api
               qo_onboard_sample_code_seed.h
               qo_onboard_sample_code_B_using_setopt_api.c)

target_link_libraries(qo_onboard_sample_code_B_using_setopt_api
                      spdlog::spdlog
                      rd_seed::rd_seed
                      qo_onboard_c)

########################################
# qo_onboard_sample_code_C_minimal_get32bytes
########################################

add_executable(qo_onboard_sample_code_C_minimal_get32bytes
               qo_onboard_sample_code_C_minimal_get32bytes.c)

target_link_libraries(qo_onboard_sample_code_C_minimal_get32bytes
                      spdlog::spdlog
                      rd_seed::rd_seed
                      qo_onboard_c)

########################################
# qo_onboard_sample_code_D_minimal_coinflip
########################################

add_executable(qo_onboard_sample_code_D_minimal_coinflip
               qo_onboard_sample_code_D_minimal_coinflip.c)

target_link_libraries(qo_onboard_sample_code_D_minimal_coinflip
                      spdlog::spdlog
                      rd_seed::rd_seed
                      qo_onboard_c)


########################################
# Add sample_code to package
########################################

install(FILES   README.md
                Config.md
                qo_onboard_sample_code_A_using_config_file.c
                qo_onboard_sample_code_B_using_setopt_api.c
                qo_onboard_sample_code_C_minimal_get32bytes.c
                qo_onboard_sample_code_D_minimal_coinflip.c
                qo_onboard_sample_code_seed.h
                qo_onboard_sample_config.yml
                Makefile
                qoo_samples.sh
                qoo_samples_build.sh
                qoo_samples_install.sh
                qoo_samples_run.sh
            DESTINATION
                share/sample_code
            COMPONENT
                c-library)
