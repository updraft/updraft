rm CMakeCache.txt

DESKTOP=/Users/User/Desktop
export OSG_DIR=$DESKTOP/osg_release
export DYLD_LIBRARY_PATH=$OSG_DIR/lib

cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=i386 -DOSG_VERSION_EXE=$OSG_DIR/bin/osgversion -DCMAKE_INSTALL_PREFIX=$DESKTOP/osgearth_release -DOSG_DIR=$OSG_DIR

make
make install