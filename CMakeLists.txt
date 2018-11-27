cmake_minimum_required(VERSION 3.5)
project(mstch)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  set(mstch_IS_MASTER_PROJECT OFF)
else()
  set(mstch_IS_MASTER_PROJECT ON)
endif()

# unit tests and benchmarks are off by default for mstch being a subproject
option(WITH_UNIT_TESTS "enable building unit test executable" ${mstch_IS_MASTER_PROJECT})
option(WITH_BENCHMARK "enable building benchmark executable" ${mstch_IS_MASTER_PROJECT})

set(mstch_VERSION 1.0.2)

string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" mstch_VERSION_MATCH "${mstch_VERSION}")
set(mstch_VERSION_MAJOR "${CMAKE_MATCH_1}")
set(mstch_VERSION_MINOR "${CMAKE_MATCH_2}")
set(mstch_VERSION_PATCH "${CMAKE_MATCH_3}")
set(mstch_SOVERSION "${mstch_VERSION_MAJOR}.${mstch_VERSION_MINOR}")
unset(mstch_VERSION_MATCH)

if(NOT MSVC)
  add_compile_options(-std=c++17 -Wall -Wextra -O3)
else()
  add_compile_options(/std:c++latest)
endif()

add_subdirectory(src)

add_library(mstch
    ${mstch_SOURCES})

set_target_properties(mstch
    PROPERTIES VERSION ${mstch_VERSION}
               SOVERSION ${mstch_SOVERSION})
target_include_directories(mstch
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

install(
    TARGETS mstch EXPORT mstchTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
install(
    FILES "${PROJECT_SOURCE_DIR}/include/mstch/mstch.hpp"
    DESTINATION include/mstch
    COMPONENT Devel)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mstch/mstch-config-version.cmake"
    VERSION ${mstch_VERSION}
    COMPATIBILITY AnyNewerVersion)
export(
    EXPORT mstchTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/mstch/mstch-targets.cmake"
    NAMESPACE mstch::)
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/mstch-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/mstch/mstch-config.cmake")
install(
    EXPORT mstchTargets
    FILE mstch-targets.cmake
    NAMESPACE mstch::
    DESTINATION lib/cmake/mstch)
install(FILES
    "${PROJECT_SOURCE_DIR}/cmake/mstch-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/mstch/mstch-config-version.cmake"
    DESTINATION lib/cmake/mstch
    COMPONENT Devel)

if(WITH_UNIT_TESTS)
  enable_testing()
  add_subdirectory(vendor/headerize)
  add_subdirectory(test)
endif()

if(WITH_BENCHMARK)
  # deactivate self-tests for benchmark dependency
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
  add_subdirectory(vendor/benchmark)
  add_subdirectory(benchmark)
endif()
