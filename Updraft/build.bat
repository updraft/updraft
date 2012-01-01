::
:: Script for Windows build
:: Creates Visual Studio solution in the ./build directory
::


:: Change the following declarations to right values before run!!!!

set cmake_path=cmake
set osg_dir=c:/Users/Tom/Desktop/UpdraftDependencies/osg
set osgearth_dir=c:/Users/Tom/Desktop/UpdraftDependencies/osgEarth

:: End of variables declaration -----------------------------------


cls
mkdir build
cd build

%cmake_path% -DOSG_DIR=%osg_dir% -DOSGEARTH_DIR=%osgearth_dir% ..

cd ..
