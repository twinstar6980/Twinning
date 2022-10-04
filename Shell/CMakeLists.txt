cmake_minimum_required(VERSION 3.21)

project(TwinKleS.ToolKit.Shell)

include(./cmake/setting.cmake)
include(./cmake/condition.cmake)

set(M_root_project_directory ${CMAKE_SOURCE_DIR})

add_subdirectory(./shell)
