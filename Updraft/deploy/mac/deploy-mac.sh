if [ $# != 3 ]; then
echo "Usage: deploy-mac version osg_dir osgearth_dir"
exit 0
fi
VER="$1"
OSG_DIR="$2"
OSGEARTH_DIR="$3"

function copy_framework {
echo "Copying framework $1"
cp -R /Library/Frameworks/$1.framework Contents/Frameworks/$1.framework
}

function change_dependencies {
echo "Changing dependencies in $1"
NEW_FRAMEWORKS_PATH=@executable_path/../../Frameworks
    install_name_tool -change QtCore.framework/Versions/4/QtCore $NEW_FRAMEWORKS_PATH/QtCore.framework/Versions/4/QtCore $1
    install_name_tool -change QtGui.framework/Versions/4/QtGui $NEW_FRAMEWORKS_PATH/QtGui.framework/Versions/4/QtGui $1
    install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL $NEW_FRAMEWORKS_PATH/QtOpenGL.framework/Versions/4/QtOpenGL $1
    install_name_tool -change QtXml.framework/Versions/4/QtXml $NEW_FRAMEWORKS_PATH/QtXml.framework/Versions/4/QtXml $1
    install_name_tool -change /Library/Frameworks/GDAL.framework/Versions/1.8/GDAL $NEW_FRAMEWORKS_PATH/GDAL.framework/Versions/1.8/GDAL $1
    install_name_tool -change /Library/Frameworks/PROJ.framework/Versions/4/PROJ $NEW_FRAMEWORKS_PATH/PROJ.framework/Versions/4/PROJ $1
    install_name_tool -change /Library/Frameworks/SQLite3.framework/Versions/3/SQLite3 $NEW_FRAMEWORKS_PATH/SQLite3.framework/Versions/3/SQLite3 $1
    install_name_tool -change /Library/Frameworks/GEOS.framework/Versions/3/GEOS $NEW_FRAMEWORKS_PATH/GEOS.framework/Versions/3/GEOS $1
    install_name_tool -change /Library/Frameworks/UnixImageIO.framework/Versions/D/UnixImageIO $NEW_FRAMEWORKS_PATH/UnixImageIO.framework/Versions/D/UnixImageIO $1
}

function init {
rm -rf dmg
mkdir dmg
cp -R bundle-template dmg/Updraft.app

cd dmg/Updraft.app

BUILD_DIR=../../../../build
#OSG_DIR=/Users/User/Desktop/osg_release
#OSGEARTH_DIR=/Users/User/Desktop/osgearth_release

cp -R $BUILD_DIR Contents/MacOS/build
cp -R $OSG_DIR Contents/Frameworks/osg_release
cp -R $OSGEARTH_DIR Contents/Frameworks/osgearth_release
copy_framework QtCore
copy_framework QtGui
copy_framework QtOpenGL
copy_framework QtXml
copy_framework GDAL
copy_framework SQLite3
copy_framework PROJ
copy_framework GEOS
copy_framework UnixImageIO
}

echo "Copying dependencies..."
init
#cd dmg/Updraft.app/Contents

echo "Updating references..."

change_dependencies Contents/Frameworks/QtGui.framework/QtGui
change_dependencies Contents/Frameworks/QtOpenGL.framework/QtOpenGL
change_dependencies Contents/Frameworks/QtXml.framework/QtXml
change_dependencies Contents/Frameworks/GDAL.framework/Versions/1.8/GDAL
change_dependencies Contents/Frameworks/GDAL.framework/Versions/1.8/Libraries/libogdi.dylib
change_dependencies Contents/Frameworks/SQLite3.framework/Versions/3/SQLite3
change_dependencies Contents/Frameworks/PROJ.framework/Versions/4/PROJ
change_dependencies Contents/Frameworks/GEOS.framework/Versions/3/GEOS
change_dependencies Contents/Frameworks/UnixImageIO.framework/Versions/D/UnixImageIO

# osg
for file in Contents/Frameworks/osg_release/lib/*.dylib; do
change_dependencies $file
done

# osgearth
for file in Contents/Frameworks/osgearth_release/lib/*.dylib; do
change_dependencies $file
done

# osgearth plugins
for file in Contents/Frameworks/osgearth_release/lib/osgPlugins-3.0.1/*.so; do
change_dependencies $file
done

# updraft
change_dependencies Contents/MacOS/build/updraft

# updraft libs
for file in Contents/MacOS/build/*.dylib; do
change_dependencies $file
done

# updraft plugins
for file in Contents/MacOS/build/plugins/*/*.dylib; do
change_dependencies $file
done

cd ../..

echo "Creating dmg..."

mkdir ../../build/_package
./make-dmg.sh Updraft $VER dmg
rm -rf dmg

