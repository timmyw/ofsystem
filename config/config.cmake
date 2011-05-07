set(PROJECTNAME ofsystem)

set(BD ${CMAKE_BINARY_DIR})

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_BUILD_TYPE "Release")

include(${BD}/config/functions.cmake)

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

# Defaults to 32bit platform on Win32
SET(BUILD_PLATFORM OFPLATFORM_I386)
SET(BUILD_OS OFOPSYS_WIN32)

include_directories(include ${BD}/include)

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

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic -fPIC")

ENDIF(WIN32)

# Change this to "SHARED" to create shared libraries
if(WIN32)
set(SHAREDSTATIC "STATIC")
else(WIN32)
set(SHAREDSTATIC "SHARED")
endif(WIN32)

# Add the definitions for OS and PLATFORM
add_definitions(-D${BUILD_OS} -D${BUILD_PLATFORM} -D${WORDSIZE} -DOF_${SHAREDSTATIC}_BUILD)

# Add debug/ndebug macros
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# Add the shared build flags, if necessary
if (SHAREDSTATIC STREQUAL "SHARED")
#  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared")
else (SHAREDSTATIC STREQUAL "SHARED")
endif (SHAREDSTATIC STREQUAL "SHARED")

if(WIN32)
set(bindir ${BD}/../bin)
set(libdir ${BD}/../lib)
set(incdir ${BD}/../include)
else(WIN32)
IF(NOT DEFINED(${PREFIX}))
set(PREFIX /usr/local)
endif(NOT DEFINED(${PREFIX}))
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
