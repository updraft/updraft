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

typedef OpenAirspace::Coordinate Position;

/// Init constants
/// PI/180 constant
static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
static const double RAD_TO_DEG = 1/DEG_TO_RAD;

/// Nautical miles to meter const
static const int NM_TO_M = 1852;
static const double M_TO_NM = 1/1852;

/// Foot to meters const
static const double FT_TO_M = 0.3048;
static const double M_TO_FT = 1/0.3048;

/// Earth's quatratic mean radius for WGS-84
static const double EARTH_RADIUS_IN_METERS = 6372797.560856;

/// 2pi
static const double M_2PI = 2*M_PI;

/// airspace arcs granularity
static const double ARC_GRANULARITY = 0.05;

/// Default line trnsparency
static const float DEFAULT_TRANSPARENCY = 0.1f;

/// Default elevations in ft
static const int GND = 0;
static const int ROOF = 80000;



class oaEngine {
 public:
  explicit oaEngine(MapLayerGroupInterface* LG);
  MapLayerInterface* Draw(const QString&);
  // bool Draw(const QString&);

 private:
  /// Map Layer Interface.
  MapLayerGroupInterface *mapLayerGroup;
  QVector<QTreeWidgetItem*> treeItems;

  /// Line properties
  /// width, colour
  float width;
  osg::Vec4f col;

  /// Draw polygon
  osg::Geometry* DrawPolygon(
    QList<Position>* pointsWGS,  // list of WGS polygon points
    const QList<double>* pointsGnd,  // list of the groud levels
    osg::Vec4& col,   // colour of the vertices
    osg::PrimitiveSet::Mode primitive,  // drawing mode
    int height, bool agl);   // height

  /// Draw polygon sides
  osg::Geometry* DrawPolygonSides(
    const QList<Position>* pointsWGS,
    const QList<double>* pointsGnd,
    osg::Vec4& col,
    osg::PrimitiveSet::Mode primitive,
    const int floor, const int ceiling,
    const bool floorAgl, const bool ceilingAgl);

  /// compute the WGS angle given the distance in nm
  double DistToAngle(double dist);

  /// Compute the arc between two WGS coords
  double ArcInRadians(const Position& from, const Position& to);

  /// Compute the distance between two WGS coords
  double DistanceInMeters(const Position& from, const Position& to);

  /// Insert Arc into the OGL vertex array
  void InsertArcI(const OpenAirspace::ArcI& aa,
    QList<Position>* vertexList);
  void InsertArcII(const OpenAirspace::ArcII& aa,
    QList<Position>* vertexList);
  // void InsertArcII(const Position& centre, const Position& start,
  //   const Position& end, bool cw, QList<Position>* vertexList);
  void InsertCircle(const OpenAirspace::Circle& cc,
    QList<Position>* vertexList);

  /// Insert middle of the arc - angles from/to in rads, r in degs, pos WGS
  void InsertMidArc(const Position& centre, double from,
    double to, const bool cw, const double& r,
    QList<Position>* vertexList);

  /// compute the circular coord angle given centre and point on circ 0 ontop
  double AngleRad(const Position& centre, const Position& point);

  /// Compute the point for given angle, centre and radius
  Position ComputeArcPoint(const Position& centre, double r, double partAngle);
  // double Dot(const osg::Vec2d&, const osg::Vec2d&);

  /// Set the colour and width of the line if possible
  void oaEngine::SetWidthAndColour(const OpenAirspace::Airspace* A);
};  // oaEngine
}  // Airspaces
}  // Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_
