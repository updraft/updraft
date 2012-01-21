::
:: Script for Windows build
:: Creates Visual Studio solution in the ./build directory
::


:: Before run:
:: make sure that you have set path to cmake in PATH variable
:: and also set following 2 environment variables:
:: 1)osg_dir      ...path of osg libraries (e.g.: C:\osg)
:: 2)osgearth_dir ...path of osgEarth libraries (e.g.: C:\osgEarth)

cls
mkdir build
cd build

set osg_dir=c:/osg
set osgearth_dir=c:/osgearth
cmake -G "Visual Studio 9 2008" -DOSG_DIR=%osg_dir% -DOSGEARTH_DIR=%osgearth_dir% ..

cd ..
