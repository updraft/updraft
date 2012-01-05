::
:: Script for Windows build
:: Creates Visual Studio solution in the ./build directory
::


:: Change the following declarations to right values before run!!!!

set cmake_path=cmake
set osg_dir=C:/OSG/UpdraftMapLibraries/Win32Release/osg
set osgearth_dir=C:/OSG/UpdraftMapLibraries/Win32Release/osgEarth

:: End of variables declaration -----------------------------------


cls
mkdir build
cd build

%cmake_path% -G"Visual Studio 9 2008" -DOSG_DIR=%osg_dir% -DOSGEARTH_DIR=%osgearth_dir% ..

cd ..
