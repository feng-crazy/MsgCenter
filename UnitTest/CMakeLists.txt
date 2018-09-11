############################################################################
#	  COPYRIGHT: 2009 Edan Instruments, Inc. All rights reserved
#	PROJECTNAME: Mx
#		VERSION:
#	   FILENAME: CMakeLists.txt
#		CREATED: 2010.07.12
#		 AUTHOR: 柯利达
#	DESCRIPTION:
############################################################################

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

link_directories(${PROJECT_BINARY_DIR}/lib)

link_directories("/usr/local/lib")

add_executable(test_bin main.cpp)

target_link_libraries(test_bin MFrame zmq pthread dl)


