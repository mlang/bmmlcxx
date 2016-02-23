# - Try to find LibStudXML
# Once done this will define
#  LIBSTUDXML_FOUND - System has LibStudXML
#  LIBSTUDXML_INCLUDE_DIRS - The LibStudXML include directories
#  LIBSTUDXML_LIBRARIES - The libraries needed to use LibStudXML
#  LIBSTUDXML_DEFINITIONS - Compiler switches required for using LibStudXML

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
find_package_handle_standard_args(LibStudXML DEFAULT_MSG
  LIBSTUDXML_LIBRARY LIBSTUDXML_INCLUDE_DIR)

mark_as_advanced(LIBSTUDXML_INCLUDE_DIR LIBSTUDXML_LIBRARY)

