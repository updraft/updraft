
------------   qt_and_osg_with_gui readme file   ------------

This readme describes how to create and configure Visual Studio solution and project.

First create VS solution (Qt application) named qt_and_osg_with_gui and import the source files.
Before build and run you must do the following:

---------------------------------------------------------------------------------
1)In MainWindow.cpp set the path to 3D model
---------------------------------------------------------------------------------

const char PATH[] = ...


---------------------------------------------------------------------------------
2)In project properties dialog (Alt+F7) set following:
---------------------------------------------------------------------------------


Configuration Properties - Debugging - Environment
-Set path to osg dlls e.g.:
PATH=$(QTDIR)\bin;c:\Users\Tom\Desktop\osg_32debug\bin;$(PATH)


Configuration Properties - C/C++ - General - Additional Include Directories
-Set path to osg include dir e.g.:
C:\Users\Tom\Desktop\osg_32debug\include


Configuration Properties - Linker - General - Additional Library Directories
-Set path to osg lib dir e.g.:
C:\Users\Tom\Desktop\osg_32debug\lib


Configuration Properties - Linker - Input - Additional Dependencies
-Set osg libs e.g.:
OpenThreadsd.lib
osgAnimationd.lib
osgd.lib
osgDBd.lib
osgFXd.lib
osgGAd.lib
osgManipulatord.lib
osgParticled.lib
osgPresentationd.lib
osgQtd.lib
osgShadowd.lib
osgSimd.lib
osgTerraind.lib
osgTextd.lib
osgUtild.lib
osgViewerd.lib
osgVolumed.lib
osgWidgetd.lib