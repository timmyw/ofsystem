cmake_minimum_required(VERSION 2.8)
include(${CMAKE_SOURCE_DIR}/config/config.cmake)
project(${PROJECTNAME})

message(STATUS "project: '" ${PROJECTNAME} "' in '" ${CMAKE_BINARY_DIR} "'")

add_subdirectory(lib)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)