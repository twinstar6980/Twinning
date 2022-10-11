cmake_minimum_required(VERSION 3.21)

project(TwinKleS.ToolKit.Shell)

set(M_root_project_directory ${CMAKE_SOURCE_DIR})

include(${M_root_project_directory}/cmake/setting.cmake)
include(${M_root_project_directory}/cmake/condition.cmake)

add_subdirectory(./shell)
