project(nrbf2json)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QNRBF_TARGET_NAME})