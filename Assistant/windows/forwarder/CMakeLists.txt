cmake_minimum_required(VERSION 3.14)
project(forwarder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(${PROJECT_NAME} SHARED
  "common.hpp"
  "forwarder_explorer_command.cppm"
  "library.cpp"
  "library.def"
  "resource/resource.rc"
)

apply_standard_settings(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE "/wd4324" "/wd4996")

target_link_libraries(${PROJECT_NAME} PRIVATE "Shlwapi.lib")
target_link_libraries(${PROJECT_NAME} PRIVATE "RuntimeObject.lib")
