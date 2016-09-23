#
# Example Dashboard script for CI services
#
# Used environment variable:
#  ITK_MODULE_NAME - name of the ITK external/remote module
#  ITK_SRC - ITK source directory ( already upto date )
#  ITK_REPOSITORY - local or remote repository
#  ITK_TAG - name of ITK tag or branch
#  ITK_TEST_LABEL = Label of the tests to run
#  ITK_BUILD_MODULES - Modules to configure to ON in ITK

cmake_policy(SET CMP0012 NEW)
cmake_policy(SET CMP0011 NEW)

set(itk_module "$ENV{ITK_MODULE_NAME}")

set(dashboard_no_update 0)
if(DEFINED ENV{ITK_SRC})
  set(CTEST_SOURCE_DIRECTORY "$ENV{ITK_SRC}")
  set(dashboard_no_update 1)
elseif(DEFINED ENV{ITK_REPOSITORY})
  set(dashboard_git_url "$ENV{ITK_REPOSITORY}")
  set(dashboard_git_branch "$ENV{ITK_TAG}")
  set(CTEST_DASHBOARD_ROOT "$ENV{HOME}/dash")
endif()

if(DEFINED ENV{ITK_TEST_LABEL})
  set(CTEST_TEST_ARGS INCLUDE_LABEL $ENV{ITK_TEST_LABEL})
endif()
set(dashboard_model "Experimental")
set(dashboard_track "Remote")

include( ProcessorCount )
ProcessorCount( PROCESSOR_COUNT )
if(PROCESSOR_COUNT)
  if (CTEST_CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    set( CTEST_BUILD_FLAGS -j${PROCESSOR_COUNT})
  endif()
  set( CTEST_TEST_ARGS ${CTEST_TEST_ARGS} PARALLEL_LEVEL ${PROCESSOR_COUNT} )
endif()

# this is the initial cache to use for the binary tree.
SET (dashboard_cache "

    BUILD_DOCUMENTATION:BOOL=OFF
    BUILD_EXAMPLES:BOOL=OFF
    BUILD_SHARED_LIBS:BOOL=OFF
    BUILD_TESTING:BOOL=ON
    ITK_USE_KWSTYLE:BOOL=OFF

    ITK_BUILD_DEFAULT_MODULES:BOOL=OFF
    ITKGroup_Core:BOOL=OFF
" )
foreach(var $ENV{ITK_BUILD_MODULES})
  set(dashboard_cache  "${dashboard_cache}
    Module_${var}:BOOL=ON
" )
endforeach()


list(APPEND CTEST_NOTES_FILES
  "${CMAKE_CURRENT_LIST_FILE}"
  )

include(${CTEST_SCRIPT_DIRECTORY}/../dashboard/itk_common.cmake)

