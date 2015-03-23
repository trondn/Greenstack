cmake_minimum_required(VERSION 2.8.4)
project(Greenstack)
if (APPLE)
   cmake_policy(SET CMP0042 NEW)
endif (APPLE)

if (NOT COUCHBASE_SERVER_BUILD)
  # Install flatbuffers
  IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE STRING
        "The install location" FORCE)
    LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")
  ENDIF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

  SET(CB_DOWNLOAD_DEPS True
      CACHE BOOL "Download prebuilt dependencies by default")

  list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/dependencies")
  add_subdirectory(dependencies)
endif (NOT COUCHBASE_SERVER_BUILD)

enable_testing()
include(CheckSymbolExists)
include(FindCouchbaseFlatbuffers)
include(TestBigEndian)

find_program(CCACHE ccache)
if (CCACHE)
   message(STATUS "Using ccache from ${CCACHE}")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE})
   set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE})
endif (CCACHE)

test_big_endian(WORDS_BIGENDIAN)

find_path(FLATBUFFERS_INCLUDE_DIR flatbuffers/flatbuffers.h)
if (FLATBUFFERS_INCLUDE_DIR)
   message(STATUS "Found flatbuffers in ${FLATBUFFERS_INCLUDE_DIR}")
   find_program(FLATC flatc)
   if (FLATC)
      message(STATUS "Found flatc as ${FLATC}")
      set(HAVE_FLATBUFFERS Found)
   endif(FLATC)
endif (FLATBUFFERS_INCLUDE_DIR)

# In order to allow the test suite to run on Windows put
# the .dll and .exe's in the same directory during building
# (Note this only affects the stuff during building and
# not the install directories)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if (WIN32)
   check_symbol_exists(htonll Winsock2.h HAVE_HTONLL)
   add_definitions(-DNOMINMAX)
   include_directories(BEFORE ${PROJECT_SOURCE_DIR}/platform/include/win32)
else (WIN32)
   check_symbol_exists(htonll arpa/inet.h HAVE_HTONLL)
   # @todo only set for the correct compiler
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(WIN32)

configure_file(${PROJECT_SOURCE_DIR}/libgreenstack/config.in.h
               ${PROJECT_BINARY_DIR}/libgreenstack/config.h)

include_directories(include ${PROJECT_BINARY_DIR}/libgreenstack)

add_subdirectory(include)
add_subdirectory(payload)
add_subdirectory(platform)
add_subdirectory(libgreenstack)

if (NOT DEFINED gtest_SOURCE_DIR)
  add_subdirectory(dependencies/gtest)
endif ()

add_subdirectory(tests)

install(FILES README.md
        DESTINATION share/doc/Greenstack)
