#CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

set(PROJECTNAME ofsystem)

include(${CMAKE_SOURCE_DIR}/config/functions.cmake)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(OFBUILDER "no-one")
IF (WIN32)
SET(OFBUILDER $ENV{USERNAME})
SET(OFBUILDHOST $ENV{COMPUTERNAME})
SET(OFBUILDPROC $ENV{PROCESSOR_ARCHITECTURE})
SET(OFBUILDOS $ENV{OS})
SET(OFBUILDMAC "")
ELSE(WIN32)
getsysvar(OFBUILDER whoami "")
getsysvar(OFBUILDMAC uname -m)
getsysvar(OFBUILDOS uname -s)
getsysvar(OFBUILDPROC uname -p)
getsysvar(UNAMEM uname -m)
ENDIF(WIN32)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED On)

# Defaults to 32bit platform on Win32
SET(BUILD_PLATFORM OFPLATFORM_I386)
SET(BUILD_OS OFOPSYS_WIN32)

include_directories(include ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include)

IF (WIN32)
  SET(BUILD_OS OFOPSYS_WIN32)
  SET(WORDSIZE "WORDSIZE32")
  SET(SERVER_INCLUDES )
  add_definitions(-DOFEXPORTS)
  include_directories(include \\mssdk\\include)
  set(LIBS ${LIBS} ws2_32)
ELSE(WIN32)
  SET(OSTYPE $ENV{OSTYPE})
  SET(BUILD_OS OFOPSYS_LINUX)
  SET(WORDSIZE "WORDSIZE32")
  IF (OSTYPE STREQUAL "linux")
    SET(BUILD_OS OFOPSYS_LINUX)
  ENDIF(OSTYPE STREQUAL "linux")

  IF (OSTYPE STREQUAL "linux-gnu")
    SET(BUILD_OS OFOPSYS_LINUX)
  ENDIF(OSTYPE STREQUAL "linux-gnu")

  IF (UNAMEM STREQUAL "x86_64")
    SET(BUILD_PLATFORM OFPLATFORM_X86_64)
    SET(WORDSIZE "WORDSIZE64")
  ENDIF(UNAMEM STREQUAL "x86_64")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic -fPIC -pthread")
  
  IF (CLANG)
    SET(LIBS ${LIBS} "dl pthread")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  ELSE(CLANG)
    SET(LIBS ${LIBS} "dl")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  ENDIF(CLANG)
ENDIF(WIN32)

# Change this to "SHARED" to create shared libraries
if(WIN32)
set(SHAREDSTATIC "STATIC")
else(WIN32)
set(SHAREDSTATIC "SHARED")
endif(WIN32)

# Add the definitions for OS and PLATFORM
add_definitions(-D${BUILD_OS} -D${BUILD_PLATFORM} -D${WORDSIZE} -DOF_${SHAREDSTATIC}_BUILD)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/include)
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
endif (NOT EXISTS ${CMAKE_BINARY_DIR}/include)
file(WRITE ${CMAKE_BINARY_DIR}/include/platform.i "// Generated Platform/OS defines\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#if !defined(${BUILD_PLATFORM})\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#define ${BUILD_PLATFORM}\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#endif\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#if !defined(${BUILD_OS})\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#define ${BUILD_OS}\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#endif\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#if !defined(OF_${SHAREDSTATIC}_BUILD)\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#define OF_${SHAREDSTATIC}_BUILD\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#endif\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#if !defined(${WORDSIZE})\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#define ${WORDSIZE}\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "#endif\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "\n")

# Add debug/ndebug macros
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# Add the shared build flags, if necessary
if (SHAREDSTATIC STREQUAL "SHARED")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pthread")
else (SHAREDSTATIC STREQUAL "SHARED")
endif (SHAREDSTATIC STREQUAL "SHARED")

if(WIN32)
set(bindir ${CMAKE_SOURCE_DIR}/../bin)
set(libdir ${CMAKE_SOURCE_DIR}/../lib)
set(incdir ${CMAKE_SOURCE_DIR}/../include)
else(WIN32)
IF("${PREFIX}" STREQUAL "")
set(PREFIX /usr/local)
endif("${PREFIX}" STREQUAL "")
set(bindir ${PREFIX}/bin)
set(libdir ${PREFIX}/lib)
set(incdir ${PREFIX}/include)
endif(WIN32)

function(addexec name srcs libs)
    add_executable(${name} ${srcs})
    target_link_libraries(${name} ${libs})
    install(TARGETS ${name} DESTINATION ${bindir})
endfunction(addexec)

function(addlib name libtype srcs libs)
    add_library(${name} ${libtype} ${srcs})
    target_link_libraries(${name} ${libs})
    install(TARGETS ${name} 
                    RUNTIME DESTINATION ${libdir}
                    LIBRARY DESTINATION ${libdir}
                    ARCHIVE DESTINATION ${libdir}
    )
endfunction(addlib)

message(STATUS "project: '" ${PROJECTNAME} "' in '" ${CMAKE_BINARY_DIR} "'")

add_subdirectory(lib)

file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "// CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
file(APPEND ${CMAKE_BINARY_DIR}/include/platform.i "// CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}\n")

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
