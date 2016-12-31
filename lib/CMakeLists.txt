project(ofsystem)

set (srcs
  oflogservice.cpp 
  ofaddress.cpp
  ofconfig.cpp
  offile.cpp
  ofos.cpp
  ofplatform.cpp
  ofsocket.cpp
  ofutility.cpp
  ofthrmgr.cpp
  ofmutex.cpp
  ofdll.cpp
  oftss.cpp
  ofcondvar.cpp
  ofdirent.cpp
  ofdatetime.cpp
  buffers.cpp
  ofvalue.cpp
  storageblob.cpp
  storageblobbuffer.cpp
  storageblobbuffered.cpp
  ofidentity.cpp
  ${CMAKE_BINARY_DIR}/lib/offlags.cpp
  srvidentity.cpp
  oferr.cpp
  ofvariant.cpp
  ofcommandarg.cpp
  ofconstants.cpp
  )

if (!WIN32)
   set (srcs ${srcs} terminput.cpp)
endif (!WIN32)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/lib/offlags.cpp
  COMMAND perl ${CMAKE_SOURCE_DIR}/tools/mkflags.pl -o offlags.cpp -h ${CMAKE_BINARY_DIR}/include/offlags.i -i ${CMAKE_SOURCE_DIR}/data/flags.data
  DEPENDS ${CMAKE_SOURCE_DIR}/tools/mkflags.pl ${CMAKE_SOURCE_DIR}/data/flags.data
)

# set (CMAKE_CXX_STANDARD 14)
# set (CMAKE_CXX_STANDARD_REQUIRED ON)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )

# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")

addlib(ofsystem ${SHAREDSTATIC} "${srcs}" "${LIBS}")

# set_property(TARGET ofsystem PROPERTY CXX_STANDARD 14)
# set_property(TARGET ofsystem PROPERTY CXX_STANDARD_REQUIRED ON)

# target_compile_features(ofsystem PUBLIC cxx_constexpr)

FILE(GLOB incfiles "${CMAKE_SOURCE_DIR}/include/*.[hi]")
FILE(GLOB incfiles2 "${CMAKE_BINARY_DIR}/include/*.[hi]")
install(FILES ${incfiles} DESTINATION ${incdir})
install(FILES ${incfiles2} DESTINATION ${incdir})
install(FILES ${CMAKE_BINARY_DIR}/include/offlags.i DESTINATION ${incdir})
