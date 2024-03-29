project(ITKZLIB)
include_regular_expression("^(itk|deflate|inf|trees|zconf|zlib|zutil).*$")

include_directories(BEFORE ${ITKZLIB_SOURCE_DIR})

# source files for zlib
set(ZLIB_SRCS
    adler32.c
    compress.c
    crc32.c
    deflate.c
    inflate.c
    inftrees.c
    inffast.c
    trees.c
    uncompr.c
    zutil.c
)

# for windows add the .def and .rc files to the source list
# if building shared libs
if(WIN32)
  if(BUILD_SHARED_LIBS)
    set(ZLIB_DLL 1)
    if(NOT UNIX)
      if(NOT BORLAND)
        if(NOT MINGW)
          set(ZLIB_SRCS ${ZLIB_SRCS} zlib.def zlib.rc  )
        endif()
      endif()
    endif()
  endif()
endif()

configure_file(${ITKZLIB_SOURCE_DIR}/.NoDartCoverage
               ${ITKZLIB_BINARY_DIR}/.NoDartCoverage)
configure_file(${ITKZLIB_SOURCE_DIR}/zlibDllConfig.h.in
               ${ITKZLIB_BINARY_DIR}/zlibDllConfig.h)


add_library(itkzlib ${ZLIB_SRCS})

# Apply user-defined properties to the library target.
if(ITK_LIBRARY_PROPERTIES)
  set_target_properties(itkzlib PROPERTIES ${ITK_LIBRARY_PROPERTIES})
endif()

install(TARGETS itkzlib
  RUNTIME DESTINATION ${ITK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT Development)

install(FILES
  ${ITKZLIB_SOURCE_DIR}/zlib.h
  ${ITKZLIB_SOURCE_DIR}/zconf.h
  ${ITKZLIB_SOURCE_DIR}/itk_zlib_mangle.h
  ${ITKZLIB_BINARY_DIR}/zlibDllConfig.h
  DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24}/itkzlib
  COMPONENT Development)
