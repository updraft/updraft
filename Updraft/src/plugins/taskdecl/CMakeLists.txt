cmake_minimum_required(VERSION 2.8)

SET(QT_USE_QTXML TRUE)

PLUGIN_BUILD(taskdecl)
TARGET_LINK_LIBRARIES(taskdecl turnpoints)
