cmake_minimum_required(VERSION 3.17)

project(QNrbfFormat LANGUAGES CXX VERSION 0.0.1.0)

option(QNRBF_BUILD_STATIC "Build QNrbf as a static library, please check the license" OFF)
option(QNRBF_BUILD_TEST "Build test targets" ON)
option(QNRBF_CHECK_ASSEMBLY "Check assembly name when deserialize nrbf file, turn off to be compatible with SVIP 1.0.0" OFF)

option(QSVIP_BUILD_STATIC "Build QSvip as a static library" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(qnrbf)
add_subdirectory(qsvip)

if (QNRBF_BUILD_TEST)
    add_subdirectory(test)
endif ()
