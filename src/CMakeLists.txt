#----------------------------------------------------------------------------
#
# Jerome Verbeke
#
# Created 07/09/2014
#

# Build dynamic (default) or static library
set(STATIC false CACHE BOOL "build static library")
# Make files for LANL
set(FORLANL false CACHE BOOL "prepare files for LANL/MCNP")
set(DIRFORLANL ${CMAKE_SOURCE_DIR}/../private/src_forlanl)
set(CCFILEFORLANL ${DIRFORLANL}/LLNLFission.cc)
set(FORTRANFILEFORLANL ${DIRFORLANL}/freya.F90)
# Custom "lint" build
set(LINT false CACHE BOOL "lint build of library")
# Use mcnp/mpi compatible implementation of xnormal
set(NEWXNORMAL false CACHE BOOL "MCNP/MPI compatible implementation of xnormal function")
# Performance profile build
set(PROF false CACHE BOOL "performance profile of library")
# Apply copyright
set(COPYRIGHT false CACHE BOOL "add copyright notices to files")
set(COPYRIGHTFILE "COPYRIGHT.TXT")
set(FORTCOPYRIGHTFILE "COPYRIGHT_FORTRAN.TXT")
set(FREYACOPYRIGHTFILE "COPYRIGHT_FREYA.TXT")
# Where to put the files with copyright notices
# set(COPYRIGHT_DESTINATION_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})
# set(COPYRIGHT_DESTINATION_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(COPYRIGHT_DESTINATION_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../include)
set(COPYRIGHT_DESTINATION_SRC_DIR ${CMAKE_SOURCE_DIR})

# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

SET(CMAKE_INSTALL_RPATH ${CMAKE_SOURCE_DIR}/../lib)

# Check if we have a fortran compiler
find_program(FORT NAMES $ENV{FC}
                        gfortran
                        ifort)
if(NOT FORT)
  project(Fission C CXX)
  message(STATUS
          "No Fortran compiler found: FREYA is not included")
else()
  project(Fission C CXX Fortran)
  message(STATUS
          "Fortran compiler found: FREYA is included")
  # only build FREYA if FORTRAN compiler is found
  add_definitions(-DFREYA)
endif()

# Locate source and headers for this project
include_directories(${PROJECT_SOURCE_DIR}/../include)
file(GLOB CSRC *.cc)
if(NOT FORT)
  list(REMOVE_ITEM CSRC ${PROJECT_SOURCE_DIR}/SmpFreya.cc)
else()
  file(GLOB FSRC *.F90)
endif()

#----------------------------------------------------------------------------
# Project-specific compiler flags
add_definitions(-DLLNLBUILD)

#----------------------------------------------------------------------------
# Build static or dynamic (default) library
#
if(STATIC)
  add_library(Fission STATIC ${CSRC} ${FSRC} ${HEADERS})
  message(STATUS "Build static library")
else()
  add_library(Fission SHARED ${CSRC} ${FSRC} ${HEADERS})
  message(STATUS "Build dynamic library")
endif()

#----------------------------------------------------------------------------
# Install the library to 'lib' directory
#
install(TARGETS Fission DESTINATION ${PROJECT_SOURCE_DIR}/../lib)

#----------------------------------------------------------------------------
# Add custom "lint" build
#
if(LINT)
  set (CMAKE_CXX_FLAGS "-Wall -Werror")
  if(FORT AND CMAKE_Fortran_COMPILER)
    get_filename_component(Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)
    if (${Fortran_COMPILER_NAME} MATCHES "gfortran.*")
      set (CMAKE_Fortran_FLAGS "-Wall -Werror -DWRITEL6 -DWRITEL8 -DWRITEL9 -Wno-character-truncation")
      message(STATUS "FORTRAN compiler is gfortran, adding full 'lint' flags")
    else()
      set (CMAKE_Fortran_FLAGS "-Wall -Werror -DWRITEL6 -DWRITEL8 -DWRITEL9")
      message(STATUS "FORTRAN compiler is not gfortran, 'lint' flags partially added")
    endif()
  endif()
endif()

#----------------------------------------------------------------------------
# Add -DNEWXNORMAL compiler flag to build library with mcnp/mpi compatible 
# implementation of function xnormal
#
if(NEWXNORMAL)
  add_definitions(-DNEWXNORMAL)
endif()

#----------------------------------------------------------------------------
# Add custom performance profiler build
#
if(PROF)
  set (CMAKE_CXX_FLAGS "-pg")
  if(FORT AND CMAKE_Fortran_COMPILER)
    get_filename_component(Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)
    if (${Fortran_COMPILER_NAME} MATCHES "gfortran.*")
      set (CMAKE_Fortran_FLAGS "-pg")
      message(STATUS "FORTRAN compiler is gfortran, adding full '-pg' flags")
    endif()
  endif()
endif()

INCLUDE(copyright.cmake)
