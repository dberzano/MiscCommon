#************************************************************************
#
# CMakeLists.txt
# 
# Anar Manafov A.Manafov@gsi.de
# 
#
#        version number:    $LastChangedRevision$
#        created by:        Anar Manafov
#                           2010-06-08
#        last changed by:   $LastChangedBy$ $LastChangedDate$
#
#        Copyright (c) 2010 GSI, Scientific Computing group. All rights reserved.
#*************************************************************************
project( MiscCommon )
#
# Version
#
set(MISCCOMMON_VERSION "5.0.0")
#
# Where to lookup modules
#
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
#
# API Docs
#
if(DOXYGEN_FOUND)
   configure_file( ${PROJECT_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY )
   add_custom_target(MiscCommon-api-docs ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
       COMMENT "Generating MiscCommon API docs")
endif(DOXYGEN_FOUND)
#
## Unit tests
#
if (BUILD_TESTS)
    message (STATUS "Build MiscCommon unit tests - YES")
    add_subdirectory ( ${MiscCommon_SOURCE_DIR}/tests ) 
else (BUILD_TESTS)
    message (STATUS "Build MiscCommon unit tests - NO")
endif (BUILD_TESTS)
