cmake_minimum_required (VERSION 3.0.0)
 
project (Test)
set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})
 
set (SOURCE src/main.cpp
	  src/syncer.cpp
	  src/syncer.h
	src/data.csv)
 
add_executable (${PROJECT_NAME} ${SOURCE})
