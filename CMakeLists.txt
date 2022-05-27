#
cmake_minimum_required(VERSION 3.16)

project(spears_client)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(spears_core)
add_executable(${PROJECT_NAME} app.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${SPEARS_CORE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SPEARS_CORE_LIBRARIES})
