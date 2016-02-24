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
set(LIBSTUDXML_DEFINITIONS ${PC_LIBSTUDXML_CFLAGS_OTHER})

find_path(LIBSTUDXML_INCLUDE_DIR xml/qname
  HINTS ${PC_LIBSTUDXML_INCLUDEDIR} ${PC_LIBSTUDXML_INCLUDE_DIRS}
  PATH_SUFFIXES libstudxml
)

find_library(LIBSTUDXML_LIBRARY
  NAMES studxml libstudxml
  HINTS ${PC_LIBSTUDXML_LIBDIR} ${PC_LIBSTUDXML_LIBRARY_DIRS}
)

set(LIBSTUDXML_LIBRARIES ${LIBSTUDXML_LIBRARY})
set(LIBSTUDXML_INCLUDE_DIRS ${LIBSTUDXML_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibStudXML
  FOUND_VAR LibStudXML_FOUND
  REQUIRED_VARS LIBSTUDXML_LIBRARY LIBSTUDXML_INCLUDE_DIR
  VERSION_VAR PC_LIBSTUDXML_VERSION
)

mark_as_advanced(LIBSTUDXML_INCLUDE_DIR LIBSTUDXML_LIBRARY)

