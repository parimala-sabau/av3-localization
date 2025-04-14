cmake_minimum_required(VERSION 3.16)

project(NDT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PCL_ROOT "C:/Development/PCL")
set(BOOST_DIR "C:/Development/PCL/3rdParty/Boost")

find_package(PCL 1.3 REQUIRED)
find_package(Boost 1.45.0)
include_directories(${PCL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} "C:/Development/PCL/bin" "C:/Program Files/OpenNI2/Redist" "C:/Development/PCL/3rdParty/VTK/bin")
link_directories(${PCL_LIBRARY_DIRS} "C:/Development/PCL/bin" "C:/Program Files/OpenNI2/Redist" "C:/Development/PCL/3rdParty/VTK/bin")
add_definitions(${PCL_DEFINITIONS})

message("LIBS: ${PCL_LIBRARY_DIRS}")

add_executable(NDT main.cpp)

target_link_libraries(NDT ${PCL_LIBRARIES} ${Boost_LIBRARIES})
