
:: This bat file runs cmake for OSG - debug
:: It must be in directory with OSG sources, where is the file CMakeLists.txt
:: Destination directory is ./build


:: Change following declarations to right values before run!!!!

set cmake_path="C:/Program Files (x86)/CMake 2.6/bin/cmake.exe"

set qmake_path=C:/QtSDK/Desktop/Qt/4.7.4/msvc2008/bin/qmake.exe


  ::OpenSceneGraph folders

set osg_d_dir=c:/Users/Tom/Desktop/osg_32debug
set osg_r_dir=c:/Users/Tom/Desktop/osg_32release
set osg_d_libdir=%osg_d_dir%/lib
set osg_r_libdir=%osg_r_dir%/lib


  ::Target compiler/linker/make

set cmake_generator="Visual Studio 9 2008"


  ::Debug or Release configuration

set build_type=Debug


:: End of variable declaration ----------------------------------------------------------------------



cls
mkdir build
cd build

%cmake_path% -G%cmake_generator% -DCMAKE_BUILD_TYPE=%build_type% -DQT_QMAKE_EXECUTABLE=%qmake_path% ^
 ..

cd ..