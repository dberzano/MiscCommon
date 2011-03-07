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
project( pod_protocol )

#
# Where to lookup modules
#
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

#
# Source files
#
set( SOURCE_FILES
     Protocol.cpp 
     ProtocolCommands.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${MiscCommon_LOCATION}
)

#
# pod-ssh executable
#
add_library(pod_protocol SHARED ${SOURCE_FILES})

target_link_libraries (
    pod_protocol
)

install(TARGETS pod_protocol DESTINATION lib)

