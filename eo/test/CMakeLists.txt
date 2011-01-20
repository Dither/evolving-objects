###############################################################################
##
## CMakeLists file for eo/test
##
###############################################################################


######################################################################################
### 1) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(${EO_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${EO_SOURCE_DIR}/contrib)
INCLUDE_DIRECTORIES(${EO_SOURCE_DIR}/contrib/MGE)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

######################################################################################
### 2) Specify where CMake can find the libraries
######################################################################################

LINK_DIRECTORIES(${EO_BINARY_DIR}/lib)

######################################################################################
### 3) Define your targets and link the librairies
######################################################################################

SET (TEST_LIST
  t-fitness
  t-Random
  t-bin
  t-Virus
  t-MGE
  t-MGE1bit
  t-MGE-control
  t-StateAndParser
  t-Checkpointing
  t-SSGA
  t-ExternalEO
  t-Symreg
  t-eo
  t-Replacement
  t-Select
  t-GenOp
  t-GA
  t-Real
  t-Vector
  t-ESAll
  t-PBIL
  t-FitnessAssembled
  t-FitnessAssembledEA
  t-Roulette
  t-Sharing
  t-CMAES
  t-SecondsElapsedContinue
  t-RNG
  t-EasyPSO
  t-Int
  t-InitPermutation
  t-SwapMutation
  t-ShiftMutation
  t-TwoOptMutation
  t-RingTopology
  t-SyncEasyPSO
  t-OrderXover
  t-ExtendedVelocity
  t-Logger
  t-IQRStat
  t-DualFitness
  t-Parser
  )


FOREACH (test ${TEST_LIST})
  SET ("T_${test}_SOURCES" "${test}.cpp")
ENDFOREACH (test)


IF(ENABLE_MINIMAL_CMAKE_TESTING)

  SET (MIN_TEST_LIST t-EasyPSO)
  FOREACH (mintest ${MIN_TEST_LIST})
    SET ("T_${mintest}_SOURCES" "${mintest}.cpp")
    ADD_EXECUTABLE(${mintest} ${T_${mintest}_SOURCES})
    ADD_TEST(${mintest} ${mintest})
    TARGET_LINK_LIBRARIES(${mintest} ga es cma eoutils eo)
  ENDFOREACH (mintest)

ELSEIF(ENABLE_CMAKE_TESTING)

  FOREACH (test ${TEST_LIST})
    ADD_EXECUTABLE(${test} ${T_${test}_SOURCES})
    ADD_TEST(${test} ${test})
    TARGET_LINK_LIBRARIES(${test} ga es cma eoutils eo)
    INSTALL(TARGETS ${test} RUNTIME DESTINATION share/eo/test COMPONENT test)
  ENDFOREACH (test)

ENDIF(ENABLE_MINIMAL_CMAKE_TESTING)

######################################################################################
