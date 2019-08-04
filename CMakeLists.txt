
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

file(GLOB_RECURSE SOURCE_FILES "*.cpp"  )

message(${SOURCE_FILES})
message("CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})

add_definitions(" -g -Wall -std=c++11 ")

include_directories(
	/usr/local/include
	/usr/lib64
)

add_executable(RaspServer
src/Config.cpp
src/App.cpp
src/ActionFactory.cpp
src/AppWebSocketWriter.cpp
src/Session.cpp
src/SessionManager.cpp
src/PCA9685.cpp
src/HcSr04.cpp
src/DS18B20.cpp
src/actions/NullAction.cpp
src/actions/PCA9685Action.cpp
src/actions/RelayAction.cpp
	)
target_link_libraries(RaspServer
	websockets
	pthread
	wiringPi
	uuid
	)



