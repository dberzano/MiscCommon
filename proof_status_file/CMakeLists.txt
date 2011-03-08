#************************************************************************
#
# CMakeLists.txt
# 
# Anar Manafov A.Manafov@gsi.de
# 
#
#        version number:    $LastChangedRevision$
#        created by:        Anar Manafov
#                           2011-03-07
#        last changed by:   $LastChangedBy$ $LastChangedDate$
#
#        Copyright (c) 2011 GSI, Scientific Computing group. All rights reserved.
#*************************************************************************
project( proof_status_file )

#
# Where to lookup modules
#
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

#
# Source files
#
set( SOURCE_FILES
     ProofStatusFile.cpp
)
set( SRC_HDRS
     ProofStatusFile.h
)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${MiscCommon_LOCATION}
    ${Boost_INCLUDE_DIRS}
)

set(Boost_filesystem_libs ${Boost_FILESYSTEM_LIBRARY})
if(local_boost_version VERSION_GREATER "1.33.1")
  set(Boost_filesystem_libs ${Boost_filesystem_libs} ${Boost_SYSTEM_LIBRARY})
endif(local_boost_version VERSION_GREATER "1.33.1")

#
# lib
#
if (CMAKE_GENERATOR STREQUAL "Xcode")
    add_library(proof_status_file SHARED ${SOURCE_FILES} ${SRC_HDRS})
else (CMAKE_GENERATOR STREQUAL "Xcode")
    add_library(proof_status_file SHARED ${SOURCE_FILES})
endif (CMAKE_GENERATOR STREQUAL "Xcode")

target_link_libraries (
    proof_status_file
    ${Boost_filesystem_libs}
)

install(TARGETS proof_status_file DESTINATION lib)

