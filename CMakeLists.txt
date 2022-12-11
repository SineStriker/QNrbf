cmake_minimum_required(VERSION 3.17)

project(QNrbfFormat LANGUAGES CXX VERSION 0.0.1.2)

# Options for QNrbf library, you may need to specify `QNRBF_BUILD_NRBF_ONLY`, others are recommended to keep
option(QNRBF_BUILD_STATIC "Build QNrbf as a static library, please check the license" OFF)
option(QNRBF_CHECK_ASSEMBLY "Check assembly name when deserialize nrbf file, turn off to be compatible with SVIP 1.0.0" OFF)
option(QNRBF_ENABLE_EXTRA_PARAMS "Enable svip params which are brought in after stable edition, no side-effect" ON)
option(QNRBF_BUILD_NRBF_ONLY "Skip building OpenSVIP modules and test targets, turn on if you want QNrbf to be a submodule" OFF)

# Compiling simplification
option(QNRBF_ONLY_C_STYLE "Export only C style interfaces" OFF)

# This option will be ignored if `QNRBF_BUILD_NRBF_ONLY` is on
option(QSVIP_BUILD_STATIC "Build QSvip as a static library" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set a global version variable for sub-projects to use
set(QNRBF_VERSION_VERBOSE ${PROJECT_VERSION})
set(QNRBF_TARGET_NAME ${PROJECT_NAME})
set(QSVIP_TARGET_NAME qsvip)

# Add sub-projects
if (QNRBF_BUILD_NRBF_ONLY)
    add_subdirectory(qnrbf)
else()
    add_subdirectory(qnrbf)
    add_subdirectory(qsvip)
    add_subdirectory(test)
endif()

