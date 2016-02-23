cmake_minimum_required(VERSION 3.4.1)
project(bmmlcxx VERSION 0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(LibStudXML REQUIRED)
find_package(Boost REQUIRED)

add_library(bmmlcxx STATIC bmml.cxx)
target_include_directories(bmmlcxx
  PUBLIC ${Boost_INCLUDE_DIRS} ${LIBSTUDXML_INCLUDE_DIRS}
)
target_link_libraries(bmmlcxx ${LIBSTUDXML_LIBRARIES})

# Tools
add_executable(driver driver.cxx)
target_link_libraries(driver bmmlcxx)

add_executable(catbmml catbmml.cxx)
target_link_libraries(catbmml bmmlcxx)

