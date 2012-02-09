#include "oaengine.h"

namespace Updraft {
namespace Airspaces {

MapLayerInterface* oaEngine::Draw(const QString& fileName) {
  OpenAirspace::Parser AirspaceSet(fileName);

  float TRANSPARENCY = 0.1f;

  if (mapLayerGroup != NULL) {
    osg::Geode* OAGeode = new osg::Geode();  // the root of the subscene
    osg::Vec4 col(0.0f, 0.5f, 1.0f, TRANSPARENCY);
    float width = 1;
    for (size_t i = 0; i < AirspaceSet.size(); ++i) {
      // set colour
      OpenAirspace::Airspace* A = AirspaceSet.at(i);
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
        width = A->GetPen()->width;
      }

      // Get the floor and ceiling of the airspace in ft msl
      // set the height of the ground in ff msl
      int gnd = 260;  // TODO(Monkey): get the groud level!
      int roof = 80000;
      // set the heights of the airspace in ft msl
      double ceiling = (A->GetCeiling()) ?
        ParseHeight(*A->GetCeiling(), gnd) : roof;
      if (ceiling == gnd) ceiling = roof;
      double floor = (A->GetFloor()) ?
        ParseHeight(*A->GetFloor(), gnd) : gnd;

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

        // OGL draw the geom
        osg::Geometry* geom;

        // Draw volume
        // draw top polygon
        geom = DrawPolygon(pointsWGS, col,
          osg::PrimitiveSet::POLYGON, ceiling);
        OAGeode->addDrawable(geom);
        // draw bottom poly
        if (floor > gnd) {
          geom = DrawPolygon(pointsWGS, col,
            osg::PrimitiveSet::POLYGON, floor);
          OAGeode->addDrawable(geom);
        }
        // draw the sides
        geom = DrawPolygonSides(pointsWGS, col,
          osg::PrimitiveSet::TRIANGLE_STRIP, ceiling, floor);
        OAGeode->addDrawable(geom);

        // Draw contours
        // osg::Vec4 fullCol(col.x(), col.y(), col.z(), 0.5f);
        osg::Vec4 fullCol(0.0f, 0.0f, 0.0f, 1.0f);
        // draw top polygon
        geom = DrawPolygon(pointsWGS, fullCol,
          osg::PrimitiveSet::LINE_STRIP, ceiling);
        OAGeode->addDrawable(geom);
        // draw bottom poly
        if (floor > gnd) {
          geom = DrawPolygon(pointsWGS, fullCol,
            osg::PrimitiveSet::LINE_STRIP, floor);
          OAGeode->addDrawable(geom);
        }
        /* Side contours
        geom = DrawPolygonSides(pointsWGS, fullCol,
          osg::PrimitiveSet::LINES, floor, ceiling);
        OAGeode->addDrawable(geom);*/
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
    // stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
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

osg::Geometry* oaEngine::DrawPolygon(QList<Position>* pointsWGS,
  osg::Vec4& col, osg::PrimitiveSet::Mode primitiveMode, int height) {
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

  // TODO(Monkey) : ensure convexity !!!
  /* if (primitiveMode == osg::PrimitiveSet::POLYGON) {
    double latSum = 0;
    double lonSum = 0;
    // compute the mean for starters
    for (int i = 0; i < pointsWGS->size(); ++i) {
      latSum += pointsWGS->at(i).lat;
      lonSum += pointsWGS->at(i).lon;
    }
    // insert centre of gravity (best I have so far)
    objectPlacer->createPlacerMatrix(latSum/pointsWGS->size(),
      lonSum/pointsWGS->size(), height*FT_TO_M, coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;
    vertexData->push_back(coord);
  }*/

  // Fill the OGL vertexArray
  for (int k = 0; k < pointsWGS->size(); ++k) {
    objectPlacer->createPlacerMatrix(pointsWGS->at(k).lat,
      pointsWGS->at(k).lon, height*FT_TO_M, coorTransformation);
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
  osg::Vec4& col, osg::PrimitiveSet::Mode primitiveMode, int floor,
  int ceiling) {
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

  // Fill the OGL vertexArray
  for (int k = 0; k < pointsWGS->size(); ++k) {
    objectPlacer->createPlacerMatrix(pointsWGS->at(k).lat,
      pointsWGS->at(k).lon, ceiling*FT_TO_M, coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;

    vertexData->push_back(coord);

    objectPlacer->createPlacerMatrix(pointsWGS->at(k).lat,
      pointsWGS->at(k).lon, floor*FT_TO_M, coorTransformation);
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

int oaEngine::ParseHeight(const QString& parsedString, int gnd) {
  // parse the string to number in ft
  // if none of cond hit, return 0
  int absoluteHeightInFt = gnd;
  if (parsedString.contains("FL", Qt::CaseInsensitive)) {
    // Compute the flight level = QNH1013.25 hPa MSL
    int s = parsedString.indexOf(QRegExp("[0-9]"));
    int e = parsedString.lastIndexOf(QRegExp("[0-9]"));
    int FL = parsedString.mid(s, e - s + 1).toInt();
    absoluteHeightInFt = FL * 100;
  } else if (parsedString.contains("MSL", Qt::CaseInsensitive)) {
    int s = parsedString.indexOf(QRegExp("[0-9]"));
    int e = parsedString.lastIndexOf(QRegExp("[0-9]"));
    absoluteHeightInFt = parsedString.mid(s, e - s + 1).toInt();
  } else if (parsedString.contains("AGL", Qt::CaseInsensitive)) {
    int s = parsedString.indexOf(QRegExp("[0-9]"));
    int e = parsedString.lastIndexOf(QRegExp("[0-9]"));
    absoluteHeightInFt = gnd + parsedString.mid(s, e - s + 1).toInt();
  }
  return absoluteHeightInFt;
}
}  // Airspaces
}  // Updraft
