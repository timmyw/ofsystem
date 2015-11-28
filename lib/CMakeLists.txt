project(${PROJECTNAME})

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

addlib(ofsystem ${SHAREDSTATIC} "${srcs}" "${LIBS}")

FILE(GLOB incfiles "${CMAKE_SOURCE_DIR}/include/*.[hi]")
FILE(GLOB incfiles2 "${CMAKE_BINARY_DIR}/include/*.[hi]")
install(FILES ${incfiles} ${incfiles2} DESTINATION ${incdir})
