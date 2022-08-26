cmake_minimum_required(VERSION 3.21)

project(TwinKleS.ToolKit.Core)

include(./cmake/setting.cmake)
include(./cmake/condition.cmake)

set(M_root_project_directory ${CMAKE_SOURCE_DIR})

add_subdirectory(./third_party/fmt)
add_subdirectory(./third_party/tinyxml2)
add_subdirectory(./third_party/md5)
add_subdirectory(./third_party/zlib)
add_subdirectory(./third_party/bzip2)
add_subdirectory(./third_party/libpng)
add_subdirectory(./third_party/ETCPACK)
add_subdirectory(./third_party/rg_etc1)
add_subdirectory(./third_party/PVRTCCompressor)
if(M_system_android AND CMAKE_ANDROID_ARCH_ABI MATCHES "^arm")
	add_subdirectory(./third_party/lzma.no_hardware)
else()
	add_subdirectory(./third_party/lzma)
endif()
if(M_system_windows)
	add_subdirectory(./third_party/quickjs.c_smile)
else()
	add_subdirectory(./third_party/quickjs)
endif()

add_subdirectory(./core)

add_subdirectory(./debugger)
