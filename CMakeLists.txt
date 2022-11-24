cmake_minimum_required(VERSION 3.17)

project(QNrbfFormat LANGUAGES CXX VERSION 0.0.1.0)

option(QNRBF_BUILD_STATIC off "Build QNrbf as a static library")
option(QNRBF_BUILD_TEST on "Build test targets")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
list(APPEND _qt_libs Qt${QT_VERSION_MAJOR}::Core)

file(GLOB_RECURSE _headers include/*.h include/*.cpp)
file(GLOB_RECURSE _sources src/*.h src/*.cpp)

if(QNRBF_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${_headers} ${_sources})
    target_compile_definitions(${PROJECT_NAME} PUBLIC QNRBF_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${_headers} ${_sources})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE QNRBF_LIBRARY)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}$<$<CONFIG:Debug>:d>
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${_qt_libs})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_include_directories(${PROJECT_NAME} PRIVATE src)

if (QNRBF_BUILD_TEST)
    add_subdirectory(test)
endif()