#include "oaengine.h"
#include <osgEarthUtil/ElevationManager>

namespace Updraft {
namespace Airspaces {

oaEngine::oaEngine(MapLayerGroupInterface* LG) {
  this->mapLayerGroup = LG;
  // set the defeault line width
  this->width = 1.0f;
  // set the default line colour
  col = osg::Vec4f(0.0f, 0.5f, 1.0f, DEFAULT_TRANSPARENCY);
}

MapLayerInterface* oaEngine::Draw(const QString& fileName) {
  OpenAirspace::Parser AirspaceSet(fileName);

  if (mapLayerGroup != NULL) {
    // Init the elevation manager
    osgEarth::Map* map = mapLayerGroup->getMapNode()->getMap();
    osgEarth::Util::ElevationManager* elevationMan =
      new osgEarth::Util::ElevationManager(map);

    // init the subscene
    osg::Geode* OAGeode = new osg::Geode();

    // Cycle through all the parsed airspaces
    for (size_t i = 0; i < AirspaceSet.size(); ++i) {
      // Process the Airspace
      OpenAirspace::Airspace* A = AirspaceSet.at(i);

      // set the colour of the geometry if defined
      SetWidthAndColour(A);

      // Get the floor and ceiling of the airspace in ft msl
      // set the height of the ground in ft msl
      // distinguish whether is the height value related to gnd level
      bool floorAgl = false;
      bool ceilingAgl = false;
      // set the heights of the airspace in ft msl
      int ceiling = A->ParseHeight(false, &ceilingAgl);
      if (ceiling == 0) ceiling = ROOF;
      int floor = A->ParseHeight(true, &floorAgl);

      // To destroy artefacts of two planes in one space
      double rnd = 0.05 * (qrand() % 100);
      floor += rnd;
      ceiling -= rnd;

      // array of coords to draw
      QList<Position>* pointsWGS = new QList<Position>();

      // main cycle through the geometry group
      if (A->GetGeometrySize() > 0) {
        for (int j = 0; j < A->GetGeometrySize(); ++j) {
          // get the geometric primitive type
          OpenAirspace::Geometry::GType gtype =
            A->GetGeometry().at(j)->GetGType();

          if  (gtype == OpenAirspace::Geometry::DPtype) {
              OpenAirspace::Polygon* p =
                (OpenAirspace::Polygon*)A->GetGeometry().at(j);
              pointsWGS->push_back(p->Centre());
          } else if (gtype == OpenAirspace::Geometry::DAtype) {
              OpenAirspace::ArcI* aa =
                (OpenAirspace::ArcI*)A->GetGeometry().at(j);
              InsertArcI(*aa, pointsWGS);
          } else if (gtype == OpenAirspace::Geometry::DCtype) {
              OpenAirspace::Circle* c =
                (OpenAirspace::Circle*)A->GetGeometry().at(j);
              InsertCircle(*c, pointsWGS);
          } else if (gtype == OpenAirspace::Geometry::DBtype) {
            OpenAirspace::ArcII* ab =
              (OpenAirspace::ArcII*)A->GetGeometry().at(j);
            InsertArcII(*ab, pointsWGS);
          }
        }

        // Compute the ground level
        // TODO(Monkey): Not working without internet access
        QList<double>* pointsGnd = NULL;
        if (floorAgl || ceilingAgl) {
          pointsGnd = new QList<double>();
          double res = 0;
          double addGnd;
          osgEarth::Util::ElevationManager::Technique t =
            elevationMan->getTechnique();
          elevationMan->setTechnique(
            osgEarth::Util::ElevationManager::TECHNIQUE_GEOMETRIC);
          // int b = elevationMan->getMaxTilesToCache();

          for (int k = 0; k < pointsWGS->size(); ++k) {
            elevationMan->getElevation(
              pointsWGS->at(k).lon,
              pointsWGS->at(k).lat,
              0.0001, 0, addGnd, res);
            pointsGnd->push_back(addGnd);
          }
        }

        // OGL draw the geom
        osg::Geometry* geom;
/* // volume representation not used
        // Draw volume
        // draw top polygon
        geom = DrawPolygon(
          pointsWGS,
          pointsGnd,
          col,
          osg::PrimitiveSet::POLYGON,
          ceiling);
        OAGeode->addDrawable(geom);
        // draw bottom poly
        if (floor > gnd) {
          geom = DrawPolygon(pointsWGS, pointsGnd, col,
            osg::PrimitiveSet::POLYGON, floor, floorAgl);
          OAGeode->addDrawable(geom);
        }*/
        // draw the sides
        geom = DrawPolygonSides(pointsWGS, pointsGnd, col,
          osg::PrimitiveSet::TRIANGLE_STRIP, floor, ceiling,
          floorAgl, ceilingAgl);
        OAGeode->addDrawable(geom);

        // TODO(Monkey): query the height only once!
        // Draw contours
        osg::Vec4 fullCol(col.x(), col.y(), col.z(), 0.5f);
        // osg::Vec4 fullCol(0.0f, 0.0f, 0.0f, 1.0f);
        // draw top polygon
        geom = DrawPolygon(pointsWGS, pointsGnd, fullCol,
          osg::PrimitiveSet::LINE_STRIP, ceiling, ceilingAgl);
        OAGeode->addDrawable(geom);
        // draw bottom poly
        if (floor > GND) {
          geom = DrawPolygon(pointsWGS, pointsGnd, fullCol,
            osg::PrimitiveSet::LINE_STRIP, floor, floorAgl);
          OAGeode->addDrawable(geom);
        }
        // Side contours
        /* geom = DrawPolygonSides(pointsWGS, pointsGnd, fullCol,
          osg::PrimitiveSet::LINES, floor, ceiling,
          floorAgl, ceilingAgl);
        OAGeode->addDrawable(geom);*/

        delete pointsWGS;
        pointsWGS = NULL;

        if (pointsGnd) {
          delete pointsGnd;
          pointsGnd = NULL;
        }
      }
    }
    // change the thickness of the line
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(width);
    // set geode params
    osg::StateSet* stateSet = OAGeode->getOrCreateStateSet();
    stateSet->setAttributeAndModes(linewidth,
      osg::StateAttribute::ON);
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
    // set transparency
    stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);

    // stateSet->setMode(GL_POLYGON_OFFSET_FILL, osg::StateAttribute::ON);
    // stateSet->setMode(GL_POLYGON_OFFSET_LINE, osg::StateAttribute::ON);
    // stateSet->setMode(GL_POLYGON_SMOOTH, osg::StateAttribute::ON);


    QString displayName = fileName.left(fileName.indexOf('.'));
    int cuntSlashes = displayName.count('/');
    displayName = displayName.section('/', cuntSlashes, cuntSlashes);

    return mapLayerGroup->insertMapLayer(OAGeode, displayName);
  }
  return NULL;
}

osg::Geometry* oaEngine::DrawPolygon(
  QList<Position>* pointsWGS,
  const QList<double>* pointsGnd,
  osg::Vec4& col,
  osg::PrimitiveSet::Mode primitiveMode,
  int height,
  bool agl) {
  // Draw the closed polygon. In case of filled polygon,
  // ensure of its convexity (it's TRIANGLE_FAN)

  // create map placer: to draw in the map
  osgEarth::Util::ObjectPlacer* objectPlacer =
    mapLayerGroup->getObjectPlacer();
  // init the geometry
  osg::Geometry* geom = new osg::Geometry();
  // OAGeode->addDrawable(geom);
  geom->setUseDisplayList(false);
  // we will be drawing polygons
  osg::DrawArrays* drawArrayLines = new
    osg::DrawArrays(primitiveMode);
  geom->addPrimitiveSet(drawArrayLines);
  // create vertex array for the lines
  osg::Vec3Array* vertexData = new osg::Vec3Array();
  geom->setVertexArray(vertexData);
  osg::Vec4Array* colors = new osg::Vec4Array;
  // transformation matrix & coords
  osg::Matrixd coorTransformation;
  osg::Vec3 coord;
  double addGnd = 0;

  // Fill the OGL vertexArray
  for (int k = 0; k < pointsWGS->size(); ++k) {
    if (agl && pointsGnd
      && pointsGnd->size() == pointsWGS->size()) {
      addGnd = pointsGnd->at(k);
    }
    objectPlacer->createPlacerMatrix(
      pointsWGS->at(k).lat,
      pointsWGS->at(k).lon,
      (height*FT_TO_M) + addGnd,
      coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;

    vertexData->push_back(coord);
  }
  colors->push_back(col);

  drawArrayLines->setFirst(0);
  drawArrayLines->setCount(vertexData->size());
  geom->setColorArray(colors);
  geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
  return geom;
}

osg::Geometry* oaEngine::DrawPolygonSides(const QList<Position>* pointsWGS,
  const QList<double>* pointsGnd,
  osg::Vec4& col,
  osg::PrimitiveSet::Mode primitiveMode,
  const int floor, const int ceiling,
  const bool floorAgl, const bool ceilingAgl) {
  // create map placer: to draw in the map
  osgEarth::Util::ObjectPlacer* objectPlacer =
    mapLayerGroup->getObjectPlacer();
  // init the geometry
  osg::Geometry* geom = new osg::Geometry();
  // OAGeode->addDrawable(geom);
  geom->setUseDisplayList(false);
  // we will be drawing polygons
  osg::DrawArrays* drawArrayLines = new
    osg::DrawArrays(primitiveMode);
  geom->addPrimitiveSet(drawArrayLines);
  // create vertex array for the lines
  osg::Vec3Array* vertexData = new osg::Vec3Array();
  geom->setVertexArray(vertexData);
  osg::Vec4Array* colors = new osg::Vec4Array;
  // transformation matrix & coords
  osg::Matrixd coorTransformation;
  osg::Vec3 coord;

  double addFloorGnd = 0;
  double addCeilingGnd = 0;

  // Fill the OGL vertexArray
  for (int k = 0; k < pointsWGS->size(); ++k) {
    if (ceilingAgl && pointsGnd
      && pointsGnd->size() == pointsWGS->size()) {
      addCeilingGnd = pointsGnd->at(k);
    }
    objectPlacer->createPlacerMatrix(pointsWGS->at(k).lat,
      pointsWGS->at(k).lon, ceiling*FT_TO_M + addCeilingGnd,
      coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;

    vertexData->push_back(coord);

    if (floorAgl && pointsGnd
      && pointsGnd->size() == pointsWGS->size()) {
      addFloorGnd = pointsGnd->at(k);
    }
    objectPlacer->createPlacerMatrix(pointsWGS->at(k).lat,
      pointsWGS->at(k).lon, floor*FT_TO_M + addFloorGnd,
      coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;

    vertexData->push_back(coord);

    if (primitiveMode == osg::PrimitiveSet::LINES)
      colors->push_back(col);
  }
  colors->push_back(col);

  drawArrayLines->setFirst(0);
  drawArrayLines->setCount(vertexData->size());
  geom->setColorArray(colors);
  geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
  return geom;
}

double oaEngine::DistToAngle(double dInNauticalMiles) {
  double dInMeters = NM_TO_M * dInNauticalMiles;
  double angleRad = asin(dInMeters/EARTH_RADIUS_IN_METERS);
  double angleDeg = angleRad * RAD_TO_DEG;
  return angleDeg;
}

double oaEngine::ArcInRadians(const Position& from, const Position& to) {
  double latitudeArc  = (from.lat - to.lat) * DEG_TO_RAD;
  double longitudeArc = (from.lon - to.lon) * DEG_TO_RAD;
  double latitudeH = sin(latitudeArc * 0.5);
  latitudeH *= latitudeH;
  double lontitudeH = sin(longitudeArc * 0.5);
  lontitudeH *= lontitudeH;
  double tmp = cos(from.lat*DEG_TO_RAD) * cos(to.lat*DEG_TO_RAD);
  return 2.0 * asin(sqrt(latitudeH + tmp*lontitudeH));
}

double oaEngine::DistanceInMeters(const Position& from, const Position& to) {
  return EARTH_RADIUS_IN_METERS*ArcInRadians(from, to);
}

void oaEngine::InsertArcI(const OpenAirspace::ArcI& aa,
  QList<Position>* vertexList) {
  // init
  const Position centre = aa.Centre();
  const double fromAngle = aa.Start() * DEG_TO_RAD;
  const double toAngle = aa.End() * DEG_TO_RAD;
  const bool cw = aa.CW();
  const double r = DistToAngle(aa.R());

  // insert the Arc
  InsertMidArc(centre, fromAngle, toAngle, cw, r, vertexList);
}
void oaEngine::InsertArcII(const OpenAirspace::ArcII& ab,
  QList<Position>* vertexList) {
  // init:
  const Position centre = ab.Centre();
  const Position start = ab.Start();
  const Position end = ab.End();
  const bool cw = ab.CW();

  // compute the radius
  double r = DistToAngle(DistanceInMeters(start, centre)/NM_TO_M);

  // compute the angle of the arc
  double a1 = AngleRad(centre, start);
  double a2 = AngleRad(centre, end);

  // insert arc points coordinates
  // insert start
  vertexList->push_back(ab.Start());

  // mid points
  InsertMidArc(centre, a1, a2, cw, r, vertexList);

  // insert end
  vertexList->push_back(ab.End());
}

void oaEngine::InsertCircle(const OpenAirspace::Circle &cc,
  QList<Position>* vertexList) {
  const double r = DistToAngle(cc.R());
  for (double k = 0; k < M_2PI; k += ARC_GRANULARITY) {
    vertexList->push_back(ComputeArcPoint(cc.Centre(), r, k));
  }
  // close the loop
  vertexList->push_back(ComputeArcPoint(cc.Centre(), r, 0));
}

void oaEngine::InsertMidArc(const Position& centre, double from,
  double to, const bool cw, const double& r,
  QList<Position>* vertexList) {
  if (cw) {
    if (to < from) to += M_2PI;
    for (double angle = from; angle < to;
      angle += ARC_GRANULARITY) {
      Position arcPoint = ComputeArcPoint(centre, r, angle);
      vertexList->push_back(arcPoint);
    }
  } else {
    if (from < to) from += M_2PI;
    for (double angle = from; angle > to;
      angle -= ARC_GRANULARITY) {
      // if (angle < M_PI) angle += M_2PI;
      Position arcPoint = ComputeArcPoint(centre, r, angle);
      vertexList->push_back(arcPoint);
    }
  }
}

double oaEngine::AngleRad(const Position& centre, const Position& point) {
  double lat = point.lat - centre.lat;
  double lon = (point.lon - centre.lon)*cos(point.lat*DEG_TO_RAD);
  double a;
  a = atan2(lon, lat);
  return a;
}

Position oaEngine::ComputeArcPoint(const Position& centre, double r,
  double partAngle) {
  double lon = sin(partAngle) * r;
  double lat = cos(partAngle) * r;
  Position result;
  result.lat = centre.lat + lat;
  result.lon = centre.lon + lon / cos(result.lat*DEG_TO_RAD);
  return result;
}

void oaEngine::SetWidthAndColour(const OpenAirspace::Airspace* A) {
  if (A->GetBrush()) {
    float r = A->GetBrush()->R/255.0f;
    float g = A->GetBrush()->G/255.0f;
    float b = A->GetBrush()->B/255.0f;
    if ( r >= 0 && g >= 0 && b >= 0)
      col.set(r, g, b, col.w());
  }
  if (A->GetPen()) {
    float r = A->GetPen()->R/255.0f;
    float g = A->GetPen()->G/255.0f;
    float b = A->GetPen()->B/255.0f;
    if ( r >= 0 && g >= 0 && b >= 0)
      col.set(r, g, b, col.w());
    this->width = A->GetPen()->width;
  }
}
}  // Airspaces
}  // Updraft
