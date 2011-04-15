cmake_minimum_required(VERSION 2.6)
include(config/config.cmake)
project(${PROJECTNAME})

message(STATUS "project: '" ${PROJECTNAME} "' in '" ${BD} "'")

add_subdirectory(lib)
