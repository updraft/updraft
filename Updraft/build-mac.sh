rm -rf build

BASEDIR=/Users/User/Desktop

export OSG_DIR=$BASEDIR/osg_release

osgearth_dir=$BASEDIR/osgearth_release

mkdir build
cd build

cmake .. -DOSG_DIR=$OSG_DIR -DOSGEARTH_DIR=$osgearth_dir -DCMAKE_OSX_ARCHITECTURES=i386 -DQUICK_BUILD=ON

make 
make install
