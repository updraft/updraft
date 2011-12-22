# Based on FindosgDB.cmake from cmake 2.8.6.

include(Findosg_functions)
OSG_FIND_PATH   (OSGQT osgQt/GraphicsWindowQt)
OSG_FIND_LIBRARY(OSGQT osgQt)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgQt DEFAULT_MSG
    OSGQT_LIBRARY OSGQT_INCLUDE_DIR)
