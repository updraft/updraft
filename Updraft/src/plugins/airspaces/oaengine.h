#ifndef UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_
#define UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_


#include <math.h>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <QString>
#include <QtGui>

#include "../../pluginbase.h"
#include "../../libraries/openairspace/openairspace.h"



namespace Updraft {
namespace Airspaces {

typedef OpenAirspace::Airspace::Coordinate Position;

/// PI/180 constant
static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
static const double RAD_TO_DEG = 1/DEG_TO_RAD;

/// Nautical miles to meter const
static const int NM_TO_M = 1852;

/// Earth's quatratic mean radius for WGS-84
static const double EARTH_RADIUS_IN_METERS = 6372797.560856;

class oaEngine {
 public:
  explicit oaEngine(MapLayerGroupInterface* LG) {mapLayerGroup = LG;}
  MapLayerInterface* Draw(const QString&);
  // bool Draw(const QString&);

 private:
  /// Map Layer Interface.
  MapLayerGroupInterface *mapLayerGroup;
  QVector<QTreeWidgetItem*> treeItems;

  /// Draw polygon
  bool Polygon();

  /// compute the WGS angle given the distance in nm
  double DistToAngle(double dist);

  /// Compute the arc between two WGS coords
  double ArcInRadians(const Position& from, const Position& to);

  /// Compute the distance between two WGS coords
  double DistanceInMeters(const Position& from, const Position& to);

  /// Insert Arc into the OGL vertex array
  // void InsertArcII(const Position& centre,
  //   const Position& start, const Position& end,
  //   const osg::Vec4& col, osg::Vec4Array*
  //     vertexArray, osg::Vec4Array* colourArray);
  void InsertArcII(const Position& centre, const Position& start,
    const Position& end, bool cw, QList<Position>* vertexList, int&);

  /// compute the circular coord angle given centre and point on circ. 0 ontop
  double AngleRad(const Position& centre, const Position& point);

  /// Compute the point for given angle, centre and radius
  Position ComputeArcPoint(const Position& centre, double r, double partAngle);
  // double Dot(const osg::Vec2d&, const osg::Vec2d&);

  /// Split the arc
  // Split()
};  // oaEngine
}  // Airspaces
}  // Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_
