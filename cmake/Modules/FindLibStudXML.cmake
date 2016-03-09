#.rst:
# FindLibStudXML
# --------------
#
# Find libstudxml.
#
# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``LIBSTUDXML_INCLUDE_DIRS``
#   where to find header files.
# ``LIBSTUDXML_LIBRARIES``
#   the libraries to link against to use libstudxml.
# ``LIBSTUDXML_DEFINITIONS``
#   You should add_definitons(${LIBSTUDXML_DEFINITIONS}) before compiling code
#   that includes libstudxml files.
# ``LIBSTUDXML_FOUND``
#   If false, do not try to use libstudxml.

find_package(PkgConfig)
pkg_check_modules(PC_LIBSTUDXML QUIET libstudxml)
set(LibStudXML_DEFINITIONS ${PC_LIBSTUDXML_CFLAGS_OTHER})
find_path(LibStudXML_INCLUDE_DIR xml/qname
  HINTS ${PC_LIBSTUDXML_INCLUDEDIR} ${PC_LIBSTUDXML_INCLUDE_DIRS}
  PATH_SUFFIXES libstudxml
)

find_library(LibStudXML_LIBRARY
  NAMES studxml libstudxml
  HINTS ${PC_LIBSTUDXML_LIBDIR} ${PC_LIBSTUDXML_LIBRARY_DIRS}
)
set(LibStudXML_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)
find_library(LibStudXML_STATIC_LIBRARY
  NAMES studxml libstudxml
  HINTS ${PC_LIBSTUDXML_STATIC_LIBDIR} ${PC_LIBSTUDXML_STATIC_LIBRARY_DIRS}
)
set(CMAKE_FIND_LIBRARY_SUFFIXES ${LibStudXML_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})

set(LibStudXML_LIBRARIES ${LibStudXML_LIBRARY})
set(LibStudXML_STATIC_LIBRARIES ${LibStudXML_STATIC_LIBRARY})
set(LibStudXML_INCLUDE_DIRS ${LibStudXML_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibStudXML
  FOUND_VAR LibStudXML_FOUND
  REQUIRED_VARS LibStudXML_LIBRARY LibStudXML_INCLUDE_DIR
  VERSION_VAR PC_LIBSTUDXML_VERSION
)

mark_as_advanced(LibStudXML_INCLUDE_DIR LibStudXML_LIBRARY LibStudXML_STATIC_LIBRARY)

