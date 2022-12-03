project(qsvip LANGUAGES CXX)

set(_headers)
set(_sources)

file(GLOB_RECURSE _headers include/*.h include/*.cpp)
file(GLOB_RECURSE _sources src/*.h src/*.cpp)

if(QSVIP_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${_headers} ${_sources})
    target_compile_definitions(${PROJECT_NAME} PUBLIC QSVIP_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${_headers} ${_sources})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE QSVIP_LIBRARY)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}$<$<CONFIG:Debug>:d>
)

target_link_libraries(${PROJECT_NAME} PUBLIC qnrbf)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_include_directories(${PROJECT_NAME} PRIVATE src)
