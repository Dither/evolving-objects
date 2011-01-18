######################################################################################
### 1) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(${EO_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

######################################################################################
### 2) Define the ga target
######################################################################################

SET(EOUTILS_LIB_OUTPUT_PATH ${EO_BINARY_DIR}/lib)
SET(LIBRARY_OUTPUT_PATH ${EOUTILS_LIB_OUTPUT_PATH})

SET (EOUTILS_SOURCES
  Data.cpp
  FileMonitor.cpp
  Gnuplot.cpp
  Gnuplot1DMonitor.cpp
  Gnuplot1DSnapshot.cpp
  IntBounds.cpp
  Parser.cpp
  RealBounds.cpp
  RNG.cpp
  State.cpp
  OStreamMonitor.cpp
  Updater.cpp
  make_help.cpp
  pipecom.cpp
  Logger.cpp
  ParserLogger.cpp
  )

ADD_LIBRARY(eoutils STATIC ${EOUTILS_SOURCES})
INSTALL(TARGETS eoutils ARCHIVE DESTINATION lib COMPONENT libraries)

FILE(GLOB HDRS *.h checkpointing)
INSTALL(FILES ${HDRS} DESTINATION include/eo/utils COMPONENT headers)

######################################################################################
### 3) Optionnal
######################################################################################

SET(EOUTILS_VERSION ${GLOBAL_VERSION})
SET_TARGET_PROPERTIES(eoutils PROPERTIES VERSION "${EOUTILS_VERSION}")

######################################################################################
