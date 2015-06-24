project(vtk_sfml)

cmake_minimum_required(VERSION 2.6.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# C++11 support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

add_executable(vtk_sfml main.cpp)

# VTK includes
set(VTK_DIR "lib/VTK/build")
find_package(VTK REQUIRED)
if (VTK_FOUND)
  include(${VTK_USE_FILE})
  target_link_libraries(vtk_sfml ${VTK_LIBRARIES})
endif()

# SFML 2.2
set(SFML_ROOT "C:/Simeco/simeco/lib/SFML-2.2/build")
set(SFML_INCLUDE_DIR "C:/Simeco/simeco/lib/SFML-2.2/include")
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(vtk_sfml ${SFML_LIBRARIES})
endif()