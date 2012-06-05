
:: This bat file runs cmake for OSG Earth - debug
:: It must be in directory with OSG Earth sources, where is the file CMakeLists.txt
:: Destination directory is ./build
:: Dependencies: OSG (including CURL), GDAL


:: Change following declarations to right values before run!!!!

set cmake_path="C:/Program Files (x86)/CMake 2.6/bin/cmake.exe"

  ::OpenSceneGraph folders

set osg_d_dir=c:/Users/Tom/Desktop/osg_32debug
set osg_r_dir=c:/Users/Tom/Desktop/osg_32release
set osg_d_libdir=%osg_d_dir%/lib
set osg_r_libdir=%osg_r_dir%/lib


  ::GDAL folders

set gdal_d_dir=c:/Users/Tom/Desktop/GDAL_32debug
set gdal_r_dir=c:/Users/Tom/Desktop/GDAL_32release


  ::Target compiler/linker/make

set cmake_generator="Visual Studio 9 2008"


  ::Debug or Release configuration

set build_type=Debug


:: End of variable declaration ----------------------------------------------------------------------



cls
mkdir build
cd build

%cmake_path% -G%cmake_generator% -DCMAKE_BUILD_TYPE=%build_type% ^
 -DCURL_INCLUDE_DIR=%osg_d_dir%/dependencies/include ^
 -DCURL_LIBRARY=%osg_r_dir%/dependencies/lib/curllib.lib ^
 -DCURL_LIBRARY_DEBUG=%osg_d_dir%/dependencies/lib/curllib.lib ^
 -DGDAL_INCLUDE_DIR=%gdal_r_dir%/include ^
 -DGDAL_LIBRARY=%gdal_r_dir%/lib/gdal_i.lib ^
 -DGDAL_LIBRARY_DEBUG=%gdal_d_dir%/lib/gdal_i.lib ^
 -DZLIB_INCLUDE_DIR=%osg_r_dir%/dependencies/include ^
 -DZLIB_LIBRARY=%osg_r_dir%/dependencies/lib/zlib1.lib ^
 -DZLIB_LIBRARY_DEBUG=%osg_d_dir%/dependencies/lib/zlib1d.lib ^
 -DOPENTHREADS_LIBRARY=%osg_r_libdir%/OpenThreads.lib ^
 -DOPENTHREADS_LIBRARY_DEBUG=%osg_d_libdir%/OpenThreadsd.lib ^
 -DOSG_INCLUDE_DIR=%osg_r_dir%/include ^
 -DOSG_GEN_INCLUDE_DIR=%osg_r_dir%/include ^
 -DOSG_LIBRARY=%osg_r_libdir%/osg.lib ^
 -DOSG_LIBRARY_DEBUG=%osg_d_libdir%/osgd.lib ^
 -DOSGDB_LIBRARY=%osg_r_libdir%/osgDB.lib ^
 -DOSGDB_LIBRARY_DEBUG=%osg_d_libdir%/osgDBd.lib ^
 -DOSGFX_LIBRARY=%osg_r_libdir%/osgFX.lib ^
 -DOSGFX_LIBRARY_DEBUG=%osg_d_libdir%/osgFXd.lib ^
 -DOSGGA_LIBRARY=%osg_r_libdir%/osgGA.lib ^
 -DOSGGA_LIBRARY_DEBUG=%osg_d_libdir%/osgGAd.lib ^
 -DOSGMANIPULATOR_LIBRARY=%osg_r_libdir%/osgManipulator.lib ^
 -DOSGMANIPULATOR_LIBRARY_DEBUG=%osg_d_libdir%/osgManipulatord.lib/ ^
 -DOSGSHADOW_LIBRARY=%osg_r_libdir%/osgShadow.lib ^
 -DOSGSHADOW_LIBRARY_DEBUG=%osg_d_libdir%/osgShadowd.lib ^
 -DOSGSIM_LIBRARY=%osg_r_libdir%/osgSim.lib ^
 -DOSGSIM_LIBRARY_DEBUG=%osg_d_libdir%/osgSimd.lib ^
 -DOSGTERRAIN_LIBRARY=%osg_r_libdir%/osgTerrain.lib ^
 -DOSGTERRAIN_LIBRARY_DEBUG=%osg_d_libdir%/osgTerraind.lib ^
 -DOSGTEXT_LIBRARY=%osg_r_libdir%/osgText.lib ^
 -DOSGTEXT_LIBRARY_DEBUG=%osg_d_libdir%/osgTextd.lib ^
 -DOSGUTIL_LIBRARY=%osg_r_libdir%/osgUtil.lib ^
 -DOSGUTIL_LIBRARY_DEBUG=%osg_d_libdir%/osgUtild.lib ^
 -DOSGVIEWER_LIBRARY=%osg_r_libdir%/osgViewer.lib ^
 -DOSGVIEWER_LIBRARY_DEBUG=%osg_d_libdir%/osgViewerd.lib ^
 -DOSGWIDGET_LIBRARY=%osg_r_libdir%/osgWidget.lib ^
 -DOSGWIDGET_LIBRARY_DEBUG=%osg_d_libdir%/osgWidgetd.lib ^
..

cd ..