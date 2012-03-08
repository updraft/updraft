FILE(GLOB loggers_dirs ${CMAKE_CURRENT_SOURCE_DIR}/*)

FOREACH(logger_dir ${loggers_dirs})
	IF(IS_DIRECTORY ${logger_dir})
		GET_FILENAME_COMPONENT(library_name ${logger_dir} NAME)
		MESSAGE("---> "${library_name})
		ADD_SUBDIRECTORY(${library_name})
	ENDIF()
ENDFOREACH()
