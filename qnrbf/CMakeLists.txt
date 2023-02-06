project(qnrbf LANGUAGES CXX VERSION ${QNRBF_VERSION_VERBOSE})

set(_qt_libs)
set(_headers)
set(_sources)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
list(APPEND _qt_libs Qt${QT_VERSION_MAJOR}::Core)

file(GLOB_RECURSE _headers include/*.h include/*.cpp)
file(GLOB_RECURSE _sources src/*.h src/*.cpp)

# Use project name as default target name
if(NOT DEFINED QNRBF_TARGET_NAME)
    set(QNRBF_TARGET_NAME ${PROJECT_NAME})
endif()

# Library type, shared is recommended
if(QNRBF_BUILD_STATIC)
    add_library(${QNRBF_TARGET_NAME} STATIC ${_headers} ${_sources})
    target_compile_definitions(${QNRBF_TARGET_NAME} PUBLIC QNRBF_STATIC)
else()
    add_library(${QNRBF_TARGET_NAME} SHARED ${_headers} ${_sources})
endif()

target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_LIBRARY)

target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_VERSION="${PROJECT_VERSION}")

# Conditional compiling macros
if(QNRBF_CHECK_ASSEMBLY)
    target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_CHECK_ASSEMBLY)
endif()

if(QNRBF_ENABLE_EXTRA_PARAMS)
    target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_ENABLE_EXTRA_PARAMS)
endif()

if(QNRBF_ONLY_C_STYLE)
    target_compile_definitions(${QNRBF_TARGET_NAME} PRIVATE QNRBF_ONLY_C_STYLE)
endif()

# Other properties
set_target_properties(${QNRBF_TARGET_NAME} PROPERTIES
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
)

if(QNRBF_ONLY_C_STYLE)
    target_link_libraries(${QNRBF_TARGET_NAME} PRIVATE ${_qt_libs})
else()
    target_link_libraries(${QNRBF_TARGET_NAME} PUBLIC ${_qt_libs})
endif()

target_include_directories(${QNRBF_TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${QNRBF_INSTALL_NAME}>
)

if(QNRBF_BUILD_STATIC AND NOT QNRBF_ONLY_C_STYLE AND NOT QNRBF_INSTALL)
    target_include_directories(${QNRBF_TARGET_NAME} PUBLIC src)
else()
    target_include_directories(${QNRBF_TARGET_NAME} PRIVATE src)
endif()

# Add embedded resources
if(WIN32)
    set(WIN32_EXPORT_NAME ${QNRBF_TARGET_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "2020")
    set(WIN32_COPYRIGHT_END_YEAR "2022")
    set(WIN32_AUTHOR_NAME "Sine Striker")
    set(WIN32_FILE_DESC "QNrbfFormat")
    set(WIN32_PRODUCT_NAME "QNrbfFormat")
    configure_file(
        res/win32/Win32DllRes.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${QNRBF_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
elseif(APPLE)
    # configure mac plist
    set_target_properties(${QNRBF_TARGET_NAME} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION CXX
        MACOSX_FRAMEWORK_NAME QNrbfFormat
        MACOSX_FRAMEWORK_IDENTIFIER com.SineStriker.QNrbf
        MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

if(QNRBF_INSTALL)
    # Add install target
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(_install_dir ${CMAKE_INSTALL_PREFIX}/cmake/${QNRBF_INSTALL_NAME})

    # Add version file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${QNRBF_INSTALL_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    # Add configuration file
    configure_package_config_file(
        ${QNRBF_INSTALL_NAME}Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${QNRBF_INSTALL_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake/${QNRBF_INSTALL_NAME}
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    install(TARGETS ${QNRBF_TARGET_NAME}
        EXPORT QNrbfTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    # Install cmake files
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${QNRBF_INSTALL_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${QNRBF_INSTALL_NAME}ConfigVersion.cmake"
        DESTINATION ${_install_dir}
    )

    # Install cmake targets files
    install(EXPORT QNrbfTargets
        DESTINATION ${_install_dir}
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${QNRBF_INSTALL_NAME}
        FILES_MATCHING PATTERN "*.h"
    )
endif()