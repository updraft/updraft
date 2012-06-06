rm CMakeCache.txt

INSTALL_DIR=/Users/User/Desktop/osgearth_release
export OSG_DIR=/Users/User/Desktop/osg_release
export DYLD_LIBRARY_PATH=$OSG_DIR/lib

cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=i386 -DOSG_VERSION_EXE=$OSG_DIR/bin/osgversion -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR -DOSG_DIR=$OSG_DIR

make
make install