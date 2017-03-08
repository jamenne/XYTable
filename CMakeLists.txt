cmake_minimum_required(VERSION 3.2)

project(XYTable)
set(PROJECT_VERSION 0.0.0.0)
set(PROJECT_BRIEF "\"No description\"")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# BOOST
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.54.0 COMPONENTS system filesystem regex REQUIRED) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

if(${APPLE})

  MESSAGE( STATUS "Building for Mac OS X, switching on C++11 flags for Mac OS X/clang" )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -fPIC")

endif(${APPLE})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  MESSAGE( STATUS "Building for Linux, switching on C++11 flags for Linux/gcc" )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra -fPIC")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

add_library(LED ../LEDClass/LED.cpp ../RS232com/RS232communication.cpp)

add_library(Spectrometer ../SpectrometerClass/Spectrometer.cpp)
target_link_libraries(Spectrometer ${Boost_LIBRARIES})

add_library(SpecMeas ../SpectrometerClass/SpecMeasurement/SpecMeasurement.cpp)

add_library(MotorControl ../MotorControl/Motor.cpp)

add_library(xyTable xyTable.cpp)


add_executable( xyTableMain xyTableMain.cpp)
#add_executable( xyTableMain xyTableMainTest.cpp)
target_link_libraries( xyTableMain xyTable MotorControl Spectrometer SpecMeas LED)

