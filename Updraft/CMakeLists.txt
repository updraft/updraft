PROJECT(updraft)
FIND_PACKAGE(Qt4 REQUIRED)

cmake_minimum_required(VERSION 2.8)

SET(QT_USE_QTOPENGL TRUE)

SET(updraft__SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp ${CMAKE_SOURCE_DIR}/src/ui/mainwindow.cpp)
SET(updraft__HEADERS ${CMAKE_SOURCE_DIR}/src/ui/mainwindow.h)
SET(updraft__FORMS ${CMAKE_SOURCE_DIR}/src/ui/mainwindow.ui)
SET(updraft__RESOURCES ${CMAKE_SOURCE_DIR}/src/resources.qrc)
SET(LANG
	translations/czech.ts
)

QT4_WRAP_CPP(updraft__HEADERS_MOC ${updraft__HEADERS})
QT4_WRAP_UI(updraft__FORMS_HEADERS ${updraft__FORMS})
QT4_ADD_RESOURCES(updraft__RESOURCES_RCC ${updraft__RESOURCES})
QT4_ADD_TRANSLATION(TRANSLATIONS ${LANG})

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_CUSTOM_TARGET(style-check ALL python ${CMAKE_SOURCE_DIR}/scripts/cpplint.py ${updraft__SOURCES} ${UPDRAFT__HEADERS})

ADD_EXECUTABLE(updraft_ ${updraft__SOURCES} 
    	${updraft__HEADERS_MOC} 
    	${updraft__FORMS_HEADERS}
	${updraft__RESOURCES_RCC}
	${TRANSLATIONS})

TARGET_LINK_LIBRARIES(updraft_ ${QT_LIBRARIES})

