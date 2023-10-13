cmake_minimum_required(VERSION 3.23)

if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()
if (DEFINED ENV{VCPKG_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    message(STATUS "VCPKG_TRIPLET: $ENV{VCPKG_TRIPLET}")
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_TRIPLET}" CACHE STRING "")
endif()

project(HoudiniPlugin)

if(NOT DEFINED HOUDINI_CMAKE_CONFIG)
message(FATAL_ERROR "use -DHOUDINI_CMAKE_CONFIG=houdini_root/toolkit/cmake to setup houdini cmake project")
endif()

list(APPEND CMAKE_PREFIX_PATH "${HOUDINI_CMAKE_CONFIG}")
find_package(Houdini REQUIRED)

# find_package(Eigen3 CONFIG REQUIRED)
# message( STATUS "EIGEN3_INCLUDE_DIR = ${EIGEN3_INCLUDE_DIR}")

set(HOUDINI_HEADER_PATH "include/")

add_subdirectory(HelloHDK)
add_subdirectory(SOP_VolumeProject)
add_subdirectory(SOP_OcclusionRemover)
add_subdirectory(SOP_Normal)
add_subdirectory(SOP_WindingIsosurface)