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
  buffers.cpp
  )

if (!WIN32)
   set (srcs ${srcs} terminput.cpp)
endif (!WIN32)

addlib(ofsystem ${SHAREDSTATIC} "${srcs}" "")