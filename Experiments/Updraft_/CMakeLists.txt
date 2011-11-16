PROJECT(updraft_)
FIND_PACKAGE(Qt4 REQUIRED)

cmake_minimum_required(VERSION 2.8)

SET(QT_USE_QTOPENGL TRUE)

SET(updraft__SOURCES main.cpp mainwindow.cpp)
SET(updraft__HEADERS mainwindow.h)
SET(updraft__FORMS mainwindow.ui)
SET(updraft__RESOURCES resources.qrc)

QT4_WRAP_CPP(updraft__HEADERS_MOC ${updraft__HEADERS})
QT4_WRAP_UI(updraft__FORMS_HEADERS ${updraft__FORMS})
QT4_ADD_RESOURCES(updraft__RESOURCES_RCC ${updraft__RESOURCES})

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(updraft_ ${updraft__SOURCES} 
    	${updraft__HEADERS_MOC} 
    	${updraft__FORMS_HEADERS}
	${updraft__RESOURCES_RCC})

TARGET_LINK_LIBRARIES(updraft_ ${QT_LIBRARIES})
