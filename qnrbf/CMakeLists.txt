project(qnrbf LANGUAGES CXX)

set(_qt_libs)
set(_headers)
set(_sources)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
list(APPEND _qt_libs Qt${QT_VERSION_MAJOR}::Core)

file(GLOB_RECURSE _headers include/*.h include/*.cpp)
file(GLOB_RECURSE _sources src/*.h src/*.cpp)

if(NOT DEFINED QNRBF_TARGET_NAME)
    set(QNRBF_TARGET_NAME ${PROJECT_NAME})
endif()

if(QNRBF_BUILD_STATIC)
    add_library(${QNRBF_TARGET_NAME} STATIC ${_headers} ${_sources})
    target_compile_definitions(${QNRBF_TARGET_NAME} PUBLIC QNRBF_STATIC)
else()
    add_library(${QNRBF_TARGET_NAME} SHARED ${_headers} ${_sources})
endif()

target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_LIBRARY)

if(QNRBF_CHECK_ASSEMBLY)
    target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_CHECK_ASSEMBLY)
endif()

set_target_properties(${QNRBF_TARGET_NAME} PROPERTIES
    OUTPUT_NAME ${QNRBF_TARGET_NAME}$<$<CONFIG:Debug>:d>
)

target_link_libraries(${QNRBF_TARGET_NAME} PUBLIC ${_qt_libs})

target_include_directories(${QNRBF_TARGET_NAME} PUBLIC include)

if(QNRBF_BUILD_STATIC)
    target_include_directories(${QNRBF_TARGET_NAME} PUBLIC src)
else()
    target_include_directories(${QNRBF_TARGET_NAME} PRIVATE src)
endif()