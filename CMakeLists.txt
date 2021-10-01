cmake_minimum_required(VERSION 3.1)

include(FindPkgConfig)

project(cpp_curl)

add_executable(cpp_curl cpp_curl.cpp cJSON/cJSON.c)

find_library(LIBCURL curl)
if (NOT LIBCURL)
    message(FATAL_ERROR "Curl library not found")
endif()

target_link_libraries(cpp_curl ${LIBCURL})
