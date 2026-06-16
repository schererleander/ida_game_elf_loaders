cmake_minimum_required(VERSION 3.27)

project(wiiuldr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(ELF_COMMON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../elf_common")

list(APPEND CMAKE_MODULE_PATH "${REPO_ROOT}/cmake")

find_package(IDA)

if(NOT IDA_SDK_FOUND)
    message(FATAL_ERROR "IDA SDK not found. Set IDA_SDK_DIR or IDA_SDK.")
endif()

set(IDA_INSTALL_DIR "$ENV{IDADIR}" CACHE PATH "IDA installation directory")

if(UNIX AND NOT APPLE)
    find_library(IDA_SHARED_LIBRARY
        NAMES ida libida.so
        PATHS "${IDA_INSTALL_DIR}"
        NO_DEFAULT_PATH
    )

    if(NOT IDA_SHARED_LIBRARY)
        message(FATAL_ERROR
            "libida.so not found. Set IDADIR to your IDA installation directory.")
    endif()

    set(IDA_RUNTIME_LIBS "${IDA_SHARED_LIBRARY}")
    set(WIIU_LOADER_SUFFIX ".so")
elseif(WIN32)
    set(IDA_RUNTIME_LIBS "${IDA_LIBRARIES}")
    set(WIIU_LOADER_SUFFIX ".dll")
else()
    message(FATAL_ERROR "This CMakeLists currently supports Linux and Windows only.")
endif()

message(STATUS "IDA SDK: ${IDA_SDK_PATH}")
message(STATUS "IDA include dir: ${IDA_INCLUDE_DIR}")
message(STATUS "IDA runtime libs: ${IDA_RUNTIME_LIBS}")
message(STATUS "Wii U loader suffix: ${WIIU_LOADER_SUFFIX}")

set(SOURCES
    "${ELF_COMMON_PATH}/elf_reader.h"
    "${ELF_COMMON_PATH}/elf.h"
    cafe.h
    cafe_loader.h
    cafe_loader.cpp
    wiiu.cpp
)

add_library(wiiuldr SHARED ${SOURCES})

target_include_directories(wiiuldr PRIVATE
    "${IDA_INCLUDE_DIR}"
    "${IDA_SDK_PATH}/ldr"
    "${ELF_COMMON_PATH}"
)

set(IDA_COMPILE_DEFS "")
foreach(def ${IDA_DEFINITIONS})
    string(REGEX REPLACE "^-D" "" defname "${def}")
    list(APPEND IDA_COMPILE_DEFS "${defname}")
endforeach()

target_compile_definitions(wiiuldr PRIVATE
    ${IDA_COMPILE_DEFS}
)

target_link_libraries(wiiuldr PRIVATE
    ${IDA_RUNTIME_LIBS}
)

if(UNIX AND NOT APPLE)
    set_target_properties(wiiuldr PROPERTIES
        BUILD_RPATH "${IDA_INSTALL_DIR}"
        INSTALL_RPATH "${IDA_INSTALL_DIR}"
    )
endif()

set_target_properties(wiiuldr PROPERTIES
    OUTPUT_NAME "wiiu"
    PREFIX ""
    SUFFIX "${WIIU_LOADER_SUFFIX}"
)
