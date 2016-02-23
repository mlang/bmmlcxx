cmake_minimum_required(VERSION 3.4.1)
project(bmmlcxx VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(LibStudXML REQUIRED)
find_package(Boost REQUIRED)

add_library(bmmlcxx SHARED bmml.cxx)
target_include_directories(bmmlcxx
  PUBLIC ${Boost_INCLUDE_DIRS} ${LIBSTUDXML_INCLUDE_DIRS}
)
target_link_libraries(bmmlcxx ${LIBSTUDXML_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  SOVERSION 0
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER bmml.hxx
)

# Tools
add_executable(driver driver.cxx)
target_link_libraries(driver bmmlcxx)

add_executable(catbmml catbmml.cxx)
target_link_libraries(catbmml bmmlcxx)

install(TARGETS bmmlcxx catbmml
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
install(EXPORT ${PROJECT_NAME} DESTINATION share/${PROJECT_NAME})

