cmake_minimum_required(VERSION 2.8.4)
project(Greenstack)
if (APPLE)
   cmake_policy(SET CMP0042 NEW)
endif (APPLE)

enable_testing()
include(CheckSymbolExists)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)


if (WIN32)
   check_symbol_exists(htonll Winsock2.h HAVE_HTONLL)
   add_definitions(-DNOMINMAX)
else (WIN32)
   check_symbol_exists(htonll arpa/inet.h HAVE_HTONLL)
   # @todo only set for the correct compiler
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(WIN32)

configure_file(${PROJECT_SOURCE_DIR}/libgreenstack/config.in.h
               ${PROJECT_BINARY_DIR}/libgreenstack/config.h)

include_directories(include ${PROJECT_BINARY_DIR}/libgreenstack)

add_subdirectory(include)
add_subdirectory(platform)
add_subdirectory(libgreenstack)
add_subdirectory(tests)
