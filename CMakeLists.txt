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
# configure files
# 
configure_file( ${PROJECT_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile @ONLY )
#
# Custom targets
#
add_custom_target(MiscCommon-api-docs doxygen ${PROJECT_BINARY_DIR}/Doxyfile
    COMMENT "Generating MiscCommon API docs")

#
## Unit tests
#
if (BUILD_TESTS)
    message (STATUS "Build MiscCommon unit tests - YES")
    add_subdirectory ( ${MiscCommon_SOURCE_DIR}/tests ) 
else (BUILD_TESTS)
    message (STATUS "Build MiscCommon unit tests - NO")
endif (BUILD_TESTS)
