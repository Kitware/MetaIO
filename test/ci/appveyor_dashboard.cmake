# Client maintainer: blowekamp@mail.nih.gov
set(CTEST_SITE "appveyor")
set(CTEST_DASHBOARD_ROOT $ENV{APPVEYOR_BUILD_FOLDER}/..)
string(SUBSTRING $ENV{APPVEYOR_REPO_COMMIT} 0 7 commit)

# Extract major/minor/patch  versions

set(what "$ENV{APPVEYOR_PULL_REQUEST_TITLE}_#$ENV{APPVEYOR_PULL_REQUEST_NUMBER}")
if("$ENV{APPVEYOR_PULL_REQUEST_NUMBER}" STREQUAL "")
  set(what "$ENV{APPVEYOR_REPO_BRANCH}")
endif()
set(CTEST_CONFIGURATION_TYPE $ENV{CONFIGURATION})
set(CTEST_CMAKE_GENERATOR "$ENV{GENERATOR}")
if("${CTEST_CMAKE_GENERATOR}" STREQUAL "")
  set(CTEST_CMAKE_GENERATOR "Visual Studio 9 2008")
endif()

set(platform $ENV{PLATFORM})

set(CTEST_BUILD_NAME "$ENV{ITK_MODULE_NAME}-VS-${platform}-$ENV{CONFIGURATION}_${what}_${commit}")

list(APPEND CTEST_NOTES_FILES
  "$ENV{PROJ_SRC}/appveyor.yml"
  )

include("${CTEST_SCRIPT_DIRECTORY}/common_dashboard.cmake")
