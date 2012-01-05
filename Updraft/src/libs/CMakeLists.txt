cmake_minimum_required(VERSION 2.8)

MESSAGE("-- Configuring libs...")

FILE(GLOB LIB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/*)

message("-- Libs that will be built:")
foreach(LIB_DIR ${LIB_DIRS})
	IF(IS_DIRECTORY ${LIB_DIR})
        	GET_FILENAME_COMPONENT(LIB_NAME ${LIB_DIR} NAME)
		message ("---> "${LIB_NAME})
		ADD_SUBDIRECTORY(${LIB_NAME})
	ENDIF(IS_DIRECTORY ${LIB_DIR})
endforeach(LIB_DIR)

