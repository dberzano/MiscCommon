#************************************************************************
#
# CMakeLists.txt
# 
# Anar Manafov A.Manafov@gsi.de
# 
#
#        version number:    $LastChangedRevision$
#        created by:        Anar Manafov
#                           2011-06-09
#        last changed by:   $LastChangedBy$ $LastChangedDate$
#
#        Copyright (c) 2011 GSI, Scientific Computing group. All rights reserved.
#*************************************************************************
project( pod_sys_files )

#
# Where to lookup modules
#
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

#
# Source files
#
set( SOURCE_FILES
     PoDSysFiles.cpp
)

set( SRC_HDRS
     PoDSysFiles.h
)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${MiscCommon_LOCATION}
    ${Boost_INCLUDE_DIRS}
)

#
# lib
#
if (CMAKE_GENERATOR STREQUAL "Xcode")
    add_library(pod_sys_files SHARED ${SOURCE_FILES} ${SRC_HDRS})
else (CMAKE_GENERATOR STREQUAL "Xcode")
    add_library(pod_sys_files SHARED ${SOURCE_FILES})
endif (CMAKE_GENERATOR STREQUAL "Xcode")

target_link_libraries (
    pod_sys_files
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

install(TARGETS pod_sys_files DESTINATION lib)

