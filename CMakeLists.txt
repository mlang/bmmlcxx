cmake_minimum_required(VERSION 3.4.1)
project(bmmlcxx VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(LibStudXML REQUIRED)
find_package(Boost REQUIRED)
find_package(PythonInterp REQUIRED)
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/bmml.hxx ${PROJECT_BINARY_DIR}/bmml.cxx
  COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/codegen.py
          ${PROJECT_SOURCE_DIR}/bmml.dtd
          ${PROJECT_BINARY_DIR}/bmml.hxx ${PROJECT_BINARY_DIR}/bmml.cxx
  DEPENDS ${PROJECT_SOURCE_DIR}/codegen.py ${PROJECT_SOURCE_DIR}/bmml.dtd
)
add_library(${PROJECT_NAME} SHARED ${PROJECT_BINARY_DIR}/bmml.cxx)
target_include_directories(${PROJECT_NAME}
  PUBLIC ${Boost_INCLUDE_DIRS} ${LibStudXML_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} ${LibStudXML_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  SOVERSION 0
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/x3.hxx;${PROJECT_BINARY_DIR}/bmml.hxx"
)
add_library(${PROJECT_NAME}-static STATIC ${PROJECT_BINARY_DIR}/bmml.cxx)
target_include_directories(${PROJECT_NAME}-static
  PUBLIC ${Boost_INCLUDE_DIRS} ${LibStudXML_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}-static
  PUBLIC ${LibStudXML_STATIC_LIBRARIES}
)

# Tools
add_executable(driver driver.cxx)
target_link_libraries(driver ${PROJECT_NAME}-static)

add_executable(catbmml catbmml.cxx)
target_link_libraries(catbmml ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-static catbmml
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib COMPONENT Library
  LIBRARY DESTINATION lib COMPONENT Library
  RUNTIME DESTINATION bin COMPONENT Runtime
  PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
install(EXPORT ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

