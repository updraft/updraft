# Locate OSGEarth
# This module defines
# OSGEARTH_LIBRARY
# OSGEARTH_FOUND, if false, do not try to link to OSG
# OSGEARTH_INCLUDE_DIR, where to find the headers
#
# $OSGEARTH_DIR is an environment variable that would
# correspond to the ./configure --prefix=$OSGEARTH_DIR
#
# Based on FindOSG.cmake Created by Robert Osfield
#   with revisions by the Delta3D team
# Modified by Th3flyboy and then modified again for Updraft project

FIND_PATH(OSGEARTH_INCLUDE_DIR osgEarth/Version
    ${OSG_DIR}/include
    ${OSG_DIR}
    ${OSGDIR}/include
    ${OSGDIR}
    ${OSG_ROOT}/include
    ${OSGEARTH_DIR}/include
    ${OSGEARTH_DIR}
    ${OSGEARTHDIR}/include
    ${OSGEARTHDIR}
    ${OSGEARTH_ROOT}/include
    $ENV{OSG_DIR}/include
    $ENV{OSG_DIR}
    $ENV{OSGDIR}/include
    $ENV{OSGDIR}
    $ENV{OSG_ROOT}/include
    $ENV{OSGEARTH_DIR}/include
    $ENV{OSGEARTH_DIR}
    $ENV{OSGEARTHDIR}/include
    $ENV{OSGEARTHDIR}
    $ENV{OSGEARTH_ROOT}/include
    $ENV{CSPDEVPACK}/include
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/include
    /usr/freeware/include
)

MACRO(FIND_OSGEARTH_LIBRARY MYLIBRARY MYLIBRARYNAME)

    FIND_LIBRARY(${MYLIBRARY}
        NAMES ${MYLIBRARYNAME}
        PATHS
        ${OSG_DIR}/lib
        ${OSG_DIR}/build/lib
        ${OSG_DIR}
        ${OSGDIR}/lib
        ${OSGDIR}
        ${OSG_ROOT}/lib
        ${OSG_ROOT}/build/lib
        ${OSGEARTH_DIR}/lib
        ${OSGEARTH_DIR}/build/lib
        ${OSGEARTH_DIR}
        ${OSGEARTHDIR}/lib
        ${OSGEARTHDIR}
        ${OSGEARTH_ROOT}/lib
        ${OSGEARTH_ROOT}/build/lib
        $ENV{OSG_DIR}/lib
        $ENV{OSG_DIR}/build/lib
        $ENV{OSG_DIR}
        $ENV{OSGDIR}/lib
        $ENV{OSGDIR}
        $ENV{OSG_ROOT}/lib
        $ENV{OSG_ROOT}/build/lib
        $ENV{OSGEARTH_DIR}/lib
        $ENV{OSGEARTH_DIR}/build/lib
        $ENV{OSGEARTH_DIR}
        $ENV{OSGEARTHDIR}/lib
        $ENV{OSGEARTHDIR}
        $ENV{OSGEARTH_ROOT}/lib
        $ENV{OSGEARTH_ROOT}/build/lib
        $ENV{CSPDEVPACK}/lib
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/lib
        /usr/freeware/lib64
    )

ENDMACRO(FIND_OSGEARTH_LIBRARY LIBRARY LIBRARYNAME)

# Find release (optimized) libs
FIND_OSGEARTH_LIBRARY(OSGEARTH_LIBRARY osgEarth)
FIND_OSGEARTH_LIBRARY(OSGEARTHSYMBOLOGY_LIBRARY osgEarthSymbology)
FIND_OSGEARTH_LIBRARY(OSGEARTHUTIL_LIBRARY osgEarthUtil)
FIND_OSGEARTH_LIBRARY(OSGEARTHFEATURES_LIBRARY osgEarthFeatures)

# Find debug libs
FIND_OSGEARTH_LIBRARY(OSGEARTH_LIBRARY_DEBUG osgEarthd)
FIND_OSGEARTH_LIBRARY(OSGEARTHSYMBOLOGY_LIBRARY_DEBUG osgEarthSymbologyd)
FIND_OSGEARTH_LIBRARY(OSGEARTHUTIL_LIBRARY_DEBUG osgEarthUtild)
FIND_OSGEARTH_LIBRARY(OSGEARTHFEATURES_LIBRARY_DEBUG osgEarthFeaturesd)

SET(OSGEARTH_LIBRARIES
    ${OSGEARTH_LIBRARY}
    ${OSGEARTHSYMBOLOGY_LIBRARY}
    ${OSGEARTHUTIL_LIBRARY}
    ${OSGEARTHFEATURES_LIBRARY})
SET(OSGEARTH_LIBRARIES_DEBUG
    ${OSGEARTH_LIBRARY_DEBUG}
    ${OSGEARTHSYMBOLOGY_LIBRARY_DEBUG}
    ${OSGEARTHUTIL_LIBRARY_DEBUG}
    ${OSGEARTHFEATURES_LIBRARY_DEGUG})
SET(OSGEARTH_INCLUDE_DIRS ${OSGEARTH_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(OsgEarth DEFAULT_MSG OSGEARTH_LIBRARY OSGEARTH_INCLUDE_DIR)
