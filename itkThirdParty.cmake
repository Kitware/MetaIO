#-----------------------------------------------------------------------------
macro(ITK_THIRD_PARTY_OPTION upper lower)
  option(ITK_USE_SYSTEM_${upper} "Use the system's ${lower} library." OFF)
  mark_as_advanced(ITK_USE_SYSTEM_${upper})
  if(ITK_USE_SYSTEM_${upper})
    if(EXISTS ${CMAKE_ROOT}/Modules/Find${upper}.cmake)
      include(${CMAKE_ROOT}/Modules/Find${upper}.cmake)
    else()
      include(${ITK_SOURCE_DIR}/Utilities/Find${upper}.cmake)
    endif()
    mark_as_advanced(${upper}_INCLUDE_DIR ${upper}_LIBRARY)
    if(${upper}_FOUND)
      set(ITK_${upper}_LIBRARIES ${${upper}_LIBRARIES})
      if("${upper}" MATCHES "^PNG$")
        set(PNG_INCLUDE_DIR ${PNG_PNG_INCLUDE_DIR})
        mark_as_advanced(PNG_PNG_INCLUDE_DIR)
      endif()
    else()
      message(SEND_ERROR "ITK_USE_SYSTEM_${upper} is ON, but ${upper}_LIBRARY is NOTFOUND.")
    endif()
  else()
    set(ITK_${upper}_LIBRARIES itk${lower})
  endif()
endmacro()

#-----------------------------------------------------------------------------
# The in-tree third-party libraries are not exported.  We only need
# the include directory inside the tree.  If using a third-party
# library from the system, though, make sure the system include
# directory is consistent inside and outside the tree.
macro(ITK_THIRD_PARTY_INCLUDE upper lower)
  if(ITK_USE_SYSTEM_${upper})
    if(${upper}_INCLUDE_DIR)
      set(ITK_INCLUDE_DIRS_SYSTEM ${ITK_INCLUDE_DIRS_SYSTEM} ${${upper}_INCLUDE_DIR})
    endif()
  else()
    set(ITK_INCLUDE_DIRS_BUILD_TREE_CXX ${ITK_INCLUDE_DIRS_BUILD_TREE_CXX}
      ${ITK_SOURCE_DIR}/Utilities/${lower}
      ${ITK_BINARY_DIR}/Utilities/${lower}
      )
  endif()
endmacro()

macro(ITK_THIRD_PARTY_INCLUDE2 upper)
  if(ITK_USE_SYSTEM_${upper})
    if(${upper}_INCLUDE_DIR)
      set(ITK_INCLUDE_DIRS_SYSTEM ${ITK_INCLUDE_DIRS_SYSTEM} ${${upper}_INCLUDE_DIR})
    endif()
  endif()
endmacro()

#-----------------------------------------------------------------------------
macro(ITK_THIRD_PARTY_SUBDIR upper lower)
  if(NOT ITK_USE_SYSTEM_${upper})
    subdirs(${lower})
  endif()
endmacro()
