project(qsvip LANGUAGES CXX VERSION ${QNRBF_VERSION_VERBOSE})

set(_headers)
set(_sources)

file(GLOB_RECURSE _headers include/*.h include/*.cpp)
file(GLOB_RECURSE _sources src/*.h src/*.cpp)

# Use project name as default target name
if(NOT DEFINED QSVIP_TARGET_NAME)
    set(QSVIP_TARGET_NAME ${PROJECT_NAME})
endif()

if(QSVIP_BUILD_STATIC)
    add_library(${QSVIP_TARGET_NAME} STATIC ${_headers} ${_sources})
    target_compile_definitions(${QSVIP_TARGET_NAME} PUBLIC QSVIP_STATIC)
else()
    add_library(${QSVIP_TARGET_NAME} SHARED ${_headers} ${_sources})
endif()

target_compile_definitions(${QSVIP_TARGET_NAME} PRIVATE QSVIP_LIBRARY)

# Other properties
set_target_properties(${QSVIP_TARGET_NAME} PROPERTIES
    OUTPUT_NAME ${QSVIP_TARGET_NAME}$<$<CONFIG:Debug>:d>
)

target_link_libraries(${QSVIP_TARGET_NAME} PUBLIC qnrbf)

target_include_directories(${QSVIP_TARGET_NAME} PUBLIC include)

if(QNRBF_BUILD_STATIC)
    target_include_directories(${QSVIP_TARGET_NAME} PUBLIC src)
else()
    target_include_directories(${QSVIP_TARGET_NAME} PRIVATE src)
endif()

# Add embedded resources
if(WIN32)
    set(WIN32_EXPORT_NAME ${QSVIP_TARGET_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "2020")
    set(WIN32_COPYRIGHT_END_YEAR "2022")
    set(WIN32_AUTHOR_NAME "Sine Striker")
    set(WIN32_FILE_DESC "C++ library of XStudio svip file resolver")
    set(WIN32_PRODUCT_NAME "qsvp")
    configure_file(
            res/win32/Win32DllRes.rc.in
            ${CMAKE_CURRENT_BINARY_DIR}/res.rc
            @ONLY
    )
    target_sources(${QSVIP_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
elseif(APPLE)
    # configure mac plist
    set_target_properties(${QSVIP_TARGET_NAME} PROPERTIES
            FRAMEWORK TRUE
            FRAMEWORK_VERSION CXX
            MACOSX_FRAMEWORK_NAME qsvip
            MACOSX_FRAMEWORK_IDENTIFIER com.SineStriker.QSvip
            MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()