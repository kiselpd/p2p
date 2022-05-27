cmake_minimum_required(VERSION 3.16)

project(spears_core)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(App)
add_subdirectory(Common)
add_subdirectory(Crypto)
add_subdirectory(Event)
add_subdirectory(Server)
add_subdirectory(UI)

file(GLOB INCLUDE_DIRS
        ${APP_INCLUDE_DIRS}
        ${COMMON_INCLUDE_DIRS}
        ${CRYPTO_INCLUDE_DIRS}
        ${EVENT_INCLUDE_DIRS}
        ${SERVER_INCLUDE_DIRS}
        ${UI_INCLUDE_DIRS}
)

file(GLOB SOURCE_FILES 
        ${APP_SOURCE_FILES}
        ${COMMON_SOURCE_FILES}
        ${CRYPTO_SOURCE_FILES}
        ${EVENT_SOURCE_FILES}
        ${SERVER_SOURCE_FILES}
        ${UI_SOURCE_FILES}
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system pthread crypto)

set(SPEARS_CORE_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(SPEARS_CORE_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
