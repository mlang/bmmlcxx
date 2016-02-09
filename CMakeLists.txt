cmake_minimum_required(VERSION 3.4.1)
project(bmmlcxx VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(libstudxml_SOURCES
  xml/qname.cxx xml/parser.cxx xml/serializer.cxx xml/value-traits.cxx
)
set(libstudxml_GENX_SOURCES
  xml/details/genx/genx.c
  xml/details/genx/char-props.c
)
find_package(EXPAT REQUIRED)
list(APPEND libstudxml_SOURCES ${libstudxml_GENX_SOURCES})
add_library(studxml STATIC ${libstudxml_SOURCES})
target_include_directories(studxml PUBLIC ${EXPAT_INCLUDE_DIRS})
target_link_libraries(studxml ${EXPAT_LIBRARIES})

add_library(bmmlcxx STATIC bmml.cxx)
target_compile_features(bmmlcxx PRIVATE cxx_auto_type)
target_link_libraries(bmmlcxx studxml)

add_executable(driver driver.cxx)
target_compile_features(driver PRIVATE cxx_auto_type)
target_link_libraries(driver bmmlcxx)


