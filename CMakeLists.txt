cmake_minimum_required( VERSION 3.10.2 )

set( METAIO_FOR_ITK 1 )
set( METAIO_NAMESPACE "ITKMetaIO" )

## Define language standard configurations requiring at least C++11 standard.
if(CMAKE_CXX_STANDARD EQUAL "98" )
   message(FATAL_ERROR "CMAKE_CXX_STANDARD:STRING=98 is not supported.")
endif()

##  Set the default target properties.
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11) # Supported values are ``11``, ``14``, ``17``, etc.
endif()
if(NOT CMAKE_CXX_STANDARD_REQUIRED)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(NOT CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# When installing stand-alone version, it will use ITK's installation scripts
if(NOT ITK_INSTALL_BIN_DIR)
  set(ITK_INSTALL_BIN_DIR "/bin")
endif()
string(REGEX REPLACE "^/" "" ITK_INSTALL_BIN_DIR_CM24 "${ITK_INSTALL_BIN_DIR}")

if(NOT ITK_INSTALL_LIB_DIR)
  set(ITK_INSTALL_LIB_DIR "/lib/itkMetaIO")
endif()
string(REGEX REPLACE "^/" "" ITK_INSTALL_LIB_DIR_CM24 "${ITK_INSTALL_LIB_DIR}")

if(NOT ITK_INSTALL_INCLUDE_DIR)
  set(ITK_INSTALL_INCLUDE_DIR "/include/itkMetaIO")
endif()
string(REGEX REPLACE "^/" "" ITK_INSTALL_INCLUDE_DIR_CM24 "${ITK_INSTALL_INCLUDE_DIR}")

# ====
project( ITKMetaIO
  VERSION "0.0.1"
  DESCRIPTION "MetaImage is the text-based tagged file format for medical images that resulted. We have now extended that file format to support a variety of objects that occur in medicine such a tubes (for vessels, needles, etc.), blobs (for arbitrary shaped objects), cubes, spheres, etc. The complete library is known at MetaIO."
  LANGUAGES CXX C)

include_directories( ${ITKMetaIO_SOURCE_DIR} )
include_directories( ${ITKMetaIO_SOURCE_DIR}/src )
include_directories( ${ITKMetaIO_BINARY_DIR} )

## Replicate ITK's way of dealing with system versions of zlib
## This makes a stand-alone installation of MetaIO incompatible with the
##   ITK version of MetaIO...don't install both...bad things will happen.
include( ${ITKMetaIO_SOURCE_DIR}/itkThirdParty.cmake )
configure_file(${ITKMetaIO_SOURCE_DIR}/itkThirdParty.h.in
               ${ITKMetaIO_BINARY_DIR}/itkThirdParty.h @ONLY IMMEDIATE)
if(NOT ITK_INSTALL_NO_DEVELOPMENT)
  install(FILES
    ${ITKMetaIO_BINARY_DIR}/itkThirdParty.h
    ${ITKMetaIO_SOURCE_DIR}/itk_zlib.h
    ${ITKMetaIO_SOURCE_DIR}/metaIOConfig.h
    DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24}
    COMPONENT Development
    )
endif()
ITK_THIRD_PARTY_OPTION( ZLIB zlib )
ITK_THIRD_PARTY_SUBDIR( ZLIB itkzlib )
if(ITK_USE_SYSTEM_ZLIB)
  set(ZLIB_LIBRARIES z)
else()
  link_directories(${CMAKE_CURRENT_BINARY_DIR}/itkzlib)
  set(ZLIB_LIBRARIES itkzlib)
endif()

if(BUILD_TESTING)
  enable_testing()
else()
  option(BUILD_TESTING "Build the testing tree" ${BUILD_TESTING})
endif()

set(METAIO_INSTALL_EXPORT_NAME ${METAIO_NAMESPACE})
set(METAIO_INSTALL_RUNTIME_DIR ${ITK_INSTALL_BIN_DIR_CM24})
set(METAIO_INSTALL_LIBRARY_DIR ${ITK_INSTALL_LIB_DIR_CM24})
set(METAIO_INSTALL_ARCHIVE_DIR ${ITK_INSTALL_LIB_DIR_CM24})
set(METAIO_INSTALL_INCLUDE_DIR ${ITK_INSTALL_INCLUDE_DIR_CM24})

add_subdirectory( src )
