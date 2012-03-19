#ifndef UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_
#define UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_


#include <math.h>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Depth>
#include <QString>
#include <QtGui>
#include <osgEarthUtil/ElevationManager>

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
static const double M_TO_NM = 1./1852;

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
  QVector<MapLayerInterface*>* Draw(const QString&);
  // bool Draw(const QString&);

 private:
  /// Map Layer Interface.
  MapLayerGroupInterface *mapLayerGroup;
  QVector<QTreeWidgetItem*> treeItems;
  QVector<MapLayerInterface*>* mapLayers;
  osg::Geode* OAGeode;
  osgEarth::Util::ElevationManager* elevationMan;

  /// Settings
  SettingInterface* testSetting;

  /// Line properties
  /// width, colour
  float width;
  osg::Vec4f col;

  /// Centre of the airspace if possible
  /// where to take height
  Position* heightRefPoint;

  /// Engine settings
  /// settings of how the drawing engine behaves
  /// Turn this on to read elevation data for each
  /// of the polygon polints
  bool USE_POINTWISE_ELEVATION;
  /// Elevation tile resolution
  /// set this to 0.0001 to get the highest resolution
  /// for polygon elevation essesment
  float ELEV_TILE_RESOLUTION;
  /// Turn this on to simplify drawing by draw the
  /// polygons under ground (no ground elevation data needed)
  bool DRAW_UNDERGROUND;
  /// Turn this on to draw the top polygon face
  bool TOP_FACE;
  /// Turnt his on to draw the bottom polygon face
  bool BOTTOM_FACE;
  /// Turn this on to draw the face sides
  bool SIDE_FACE;
  /// Turn this on to draw the top polygon wireframe
  bool TOP_WIREFRAME;
  /// Turnt his on to draw the bottom polygon wireframe
  bool BOTTOM_WIREFRAME;
  /// Turn this on to draw the wireframe sides
  bool SIDE_WIREFRAME;
  /// Turn this on to draw the sides with colour gradient
  bool SIDE_COL_GRADIENT;
  /// Opacity of the surface
  float POLY_OPACITY_BOTTOM;
  float POLY_OPACITY_TOP;
  /// Opacity of the wireframe
  float WIRE_OPACITY_BOTTOM;
  float WIRE_OPACITY_TOP;
  /// Default elevations in ft
  int GND;
  int ROOF;


  /// Compute the height
  QVector<double>* ComputeHeightData(
  int* floor, int* ceiling,
  bool* floorAgl, bool* ceilingAgl,
  Position* heightRefPoint,
  OpenAirspace::Airspace* A,
  QVector<Position>* pointsWGS);

  /// Fill the OpenGL vertex arrays
  void FillOGLArrays(
  QVector<Position>* pointsWGS,
  QVector<double>* pointsGnd,
  int floor, int ceiling,
  bool floorAgl, bool ceilingAgl);

  /// Insert the geometry Layer into the array
  void PushLayer(osg::Geode* geode, const QString& displayName);

  /// Draw polygon
  osg::Geometry* DrawPolygon(
    QVector<Position>* pointsWGS,  // list of WGS polygon points
    const QVector<double>* pointsGnd,  // list of the groud levels
    osg::Vec4& col,   // colour of the vertices
    osg::PrimitiveSet::Mode primitive,  // drawing mode
    int height, bool agl, bool cw);   // height, orientation

  /// Draw polygon sides
  osg::Geometry* DrawPolygonSides(
    const QVector<Position>* pointsWGS,
    const QVector<double>* pointsGnd,
    osg::Vec4& colBot, osg::Vec4& colTop,
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
    QVector<Position>* vertexList);
  void InsertArcII(const OpenAirspace::ArcII& aa,
    QVector<Position>* vertexList);
  // void InsertArcII(const Position& centre, const Position& start,
  //   const Position& end, bool cw, QVector<Position>* vertexList);
  void InsertCircle(const OpenAirspace::Circle& cc,
    QVector<Position>* vertexList);

  /// Insert middle of the arc - angles from/to in rads, r in degs, pos WGS
  void InsertMidArc(const Position& centre, double from,
    double to, const bool cw, const double& r,
    QVector<Position>* vertexList);

  /// compute the circular coord angle given centre and point on circ 0 ontop
  /// return (-pi, +pi)
  double AngleRad(const Position& centre, const Position& point);

  /// compute the angle btw 12oo position and point for given center
  /// return (0, 2pi)
  double AngleRadPos(const Position& centre, const Position& point);

  /// Compute the point for given angle, centre and radius
  Position ComputeArcPoint(const Position& centre, double r, double partAngle);
  // double Dot(const osg::Vec2d&, const osg::Vec2d&);

  /// Set the colour and width of the line if possible
  void SetWidthAndColour(const OpenAirspace::Airspace* A);

  /// Get the orientation for given array of closed poly points
  bool IsPolyOrientationCW(QVector<Position>* pointsWGS);
};  // oaEngine
}  // Airspaces
}  // Updraft

#endif  // UPDRAFT_SRC_PLUGINS_AIRSPACES_OAENGINE_H_
