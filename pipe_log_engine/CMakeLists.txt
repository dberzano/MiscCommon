#************************************************************************
#
# CMakeLists.txt
# 
# Anar Manafov A.Manafov@gsi.de
# 
#
#        version number:    $LastChangedRevision$
#        created by:        Anar Manafov
#                           2010-05-17
#        last changed by:   $LastChangedBy$ $LastChangedDate$
#
#        Copyright (c) 2010 GSI, Scientific Computing group. All rights reserved.
#*************************************************************************
project( pipe_log_engine )

#
# Where to lookup modules
#
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

#
# Source files
#
set( SOURCE_FILES
     logEngine.cpp )

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${MiscCommon_LOCATION}
    ${Boost_INCLUDE_DIRS} )

#
# pod-ssh executable
#
add_library(pipe_log_engine SHARED ${SOURCE_FILES})

target_link_libraries (
    pipe_log_engine
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
)

install(TARGETS pipe_log_engine DESTINATION lib)

