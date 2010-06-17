#************************************************************************
#
# CMakeLists.txt
# 
# Anar Manafov A.Manafov@gsi.de
# 
#
#        version number:    $LastChangedRevision$
#        created by:        Anar Manafov
#                           2009-12-25
#        last changed by:   $LastChangedBy$ $LastChangedDate$
#
#        Copyright (c) 2009-2010 GSI GridTeam. All rights reserved.
#*************************************************************************
project( MiscCommon-tests )

include_directories(${MiscCommon_SOURCE_DIR})
#=============================================================================
add_executable(MiscCommon_test_MiscUtils Test_MiscUtils.cpp )

target_link_libraries (
    MiscCommon_test_MiscUtils
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

install(TARGETS MiscCommon_test_MiscUtils DESTINATION tests)
#=============================================================================
add_executable(MiscCommon_test_Process Test_Process.cpp )

target_link_libraries (
    MiscCommon_test_Process
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

install(TARGETS MiscCommon_test_Process DESTINATION tests)
#=============================================================================
add_executable(MiscCommon_test_SysHelper Test_SysHelper.cpp )

target_link_libraries (
    MiscCommon_test_SysHelper
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

install(TARGETS MiscCommon_test_SysHelper DESTINATION tests)
#=============================================================================
add_executable(MiscCommon_test_FindCfgFile Test_FindCfgFile.cpp )

target_link_libraries (
    MiscCommon_test_FindCfgFile
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

install(TARGETS MiscCommon_test_FindCfgFile DESTINATION tests)

