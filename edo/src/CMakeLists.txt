######################################################################################
### 1) Set all needed source files for the project
######################################################################################

FILE(GLOB HDRS *.h edo)
INSTALL(FILES ${HDRS} DESTINATION include/edo COMPONENT headers)

FILE(GLOB SOURCES *.cpp)

SET(SAMPLE_SRCS ${SOURCES} PARENT_SCOPE)

######################################################################################


######################################################################################
### 2) Where must cmake go now ?
######################################################################################

ADD_SUBDIRECTORY(utils)

######################################################################################
