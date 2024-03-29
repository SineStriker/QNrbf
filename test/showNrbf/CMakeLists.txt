project(showNrbf LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Build another QNrbf static library to query private headers
set(QNRBF_BUILD_STATIC on)
set(QNRBF_INSTALL off)
set(QNRBF_TARGET_NAME qnrbf2)
add_subdirectory(../../qnrbf qnrbf_internal)

set(_qt_libs)
set(_sources)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
list(APPEND _qt_libs Qt${QT_VERSION_MAJOR}::Widgets)

file(GLOB_RECURSE _sources gui/*.h gui/*.cpp)
list(APPEND _sources main.cpp)

add_executable(${PROJECT_NAME} WIN32 ${_sources})

target_link_libraries(${PROJECT_NAME} PUBLIC ${_qt_libs})

target_link_libraries(${PROJECT_NAME} PUBLIC qnrbf2)