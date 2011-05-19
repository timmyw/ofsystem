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
  offlags.cpp
  srvidentity.cpp
  oferr.cpp
  ofvariant.cpp
  ofcommandarg.cpp
  )

if (!WIN32)
   set (srcs ${srcs} terminput.cpp)
endif (!WIN32)

add_custom_command(OUTPUT offlags.cpp
  COMMAND perl ${BD}/tools/mkflags.pl -o offlags.cpp -h ${BD}/include/offlags.i -i ${BD}/data/flags.data
  DEPENDS ${BD}/tools/mkflags.pl ${BD}/data/flags.data
)

addlib(ofsystem ${SHAREDSTATIC} "${srcs}" "${LIBS}")

FILE(GLOB incfiles "${BD}/include/*.[hi]")
install(FILES ${incfiles} DESTINATION ${incdir})
