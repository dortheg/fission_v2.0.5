#----------------------------------------------------------------------------
#
# Jerome Verbeke
#
# Created 07/28/2014
#

# Setup the project
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(angular_correlation C)

# Locate source and headers for this project
SET(CSRC ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.c)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../../include)

# Locate libFission
FIND_LIBRARY(LFISSION Fission
             PATHS ${PROJECT_SOURCE_DIR}/../../../lib
             DOC "LLNL Fission Library"
             NO_DEFAULT_PATH
            )
IF(NOT LFISSION)
  MESSAGE(FATAL_ERROR "Could not find LLNL Fission Library libFission")
ENDIF()

# Build executable
ADD_EXECUTABLE(${PROJECT_NAME} ${CSRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LFISSION})
ADD_DEFINITIONS(-DFREYA)
