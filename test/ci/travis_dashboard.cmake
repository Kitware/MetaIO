# Client maintainer: blowekamp@mail.nih.gov
execute_process(COMMAND hostname OUTPUT_VARIABLE hostname OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CTEST_SITE "${hostname}")
set(CTEST_DASHBOARD_ROOT $ENV{TRAVIS_BUILD_DIR}/..)
get_filename_component(compiler_name $ENV{CC} NAME)
string(SUBSTRING $ENV{TRAVIS_COMMIT} 0 7 commit)

set(what "#$ENV{TRAVIS_PULL_REQUEST}")
if($ENV{TRAVIS_PULL_REQUEST} STREQUAL "false")
  set(what "$ENV{TRAVIS_BRANCH}")
endif()
set(CTEST_BUILD_NAME "$ENV{ITK_MODULE_NAME}-$ENV{TRAVIS_OS_NAME}-${compiler_name}_${what}_${commit}")

set(CTEST_CONFIGURATION_TYPE Release)
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

list(APPEND CTEST_NOTES_FILES
  "$ENV{PROJ_SRC}/.travis.yml"
  )

include("${CTEST_SCRIPT_DIRECTORY}/common_dashboard.cmake")
