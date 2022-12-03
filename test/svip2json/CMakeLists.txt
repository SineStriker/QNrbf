project(svip2json)

add_executable(${PROJECT_NAME} main.cpp QSvipModel.h QSvipModel.cpp SvipUtils.h SvipUtils.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE QNrbfFormat)