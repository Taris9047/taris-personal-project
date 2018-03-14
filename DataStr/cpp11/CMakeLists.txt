cmake_minimum_required (VERSION 2.6)
project (DataStrCpp11)

# Version number
set (DataStrCpp11_VERSION_MAJOR 1)
set (DataStrCpp11_VERSION_MINOR 0)

# Default build type
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
	set(default_build_type "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Compiler flags
set (GCC_COMPILE_FLAGS_DEBUG "-std=c++0x -ggdb3")
set (GCC_COMPILE_FLAGS_RELEASE "-std=c++0x -O4")

set (CMAKE_CXX_FLAGS_DEBUG "${GCC_COMPILE_FLAGS_DEBUG}")
set (CMAKE_CXX_FLAGS_RELEASE "${GCC_COMPILE_FLAGS_RELEASE}")

set (CMAKE_CXX_FLAGS "-std=c++0x")
message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

include_directories("./")

file (GLOB SOURCES "./*.cpp;./*.tcc;./*.hpp")

add_executable(DataStrCpp11Test test.cpp)
