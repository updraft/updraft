After running Cmake, it is neccessary to add paths to OSG and OSGEarth libraries. Namely:

Do the following for Debug and Release version settings. 

In Windows:
===========
1. Adding \bin directories to PATH in generated solution.
2. Adding \include directories to solution.
3. Adding \lib directories to solution's Additional directories for lib.
4. Adding these libs (just add all of them): (release won't have the "d" at the end)

OSG
---
OpenThreadsd.lib osgAnimationd.lib osgd.lib osgDBd.lib osgFXd.lib osgGAd.lib osgManipulatord.lib osgParticled.lib osgPresentationd.lib osgQtd.lib osgShadowd.lib osgSimd.lib osgTerraind.lib osgTextd.lib osgUtild.lib osgViewerd.lib osgVolumed.lib osgWidgetd.lib

OSGEarth
--------
osgdb_osgearth_gdald.lib osgEarthAnnotationd.lib osgEarthd.lib osgEarthFeaturesd.lib osgEarthSymbologyd.lib osgEarthUtild.lib osgdb_osgearth_feature_ogrd.lib osgdb_osgearth_feature_wfsd.lib

Additional libraries
--------------------
curllib.lib freetype237.lib giflib.lib glut32D.lib libjpegD.lib libpng13d.lib libtiffD.lib zlib1d.lib

In Linux:
=========

I don't have a clue, sorry :(
