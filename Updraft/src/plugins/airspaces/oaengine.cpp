#include "oaengine.h"

namespace Updraft {
namespace Airspaces {

MapLayerInterface* oaEngine::Draw(const QString& fileName) {
  OpenAirspace::Parser AirspaceSet(fileName);

  if (mapLayerGroup != NULL) {
    // create map placer: to draw in the map
    osgEarth::Util::ObjectPlacer* objectPlacer =
      mapLayerGroup->getObjectPlacer();

    osg::Geode* OAGeode = new osg::Geode();  // the root of the subscene
    osg::Vec4 col(0.0f, 0.5f, 1.0f, 0.5f);
    float width = 1;
    for (size_t i = 0; i < AirspaceSet.size(); ++i) {
      // init the geometry
      osg::Geometry* geom = new osg::Geometry();
      OAGeode->addDrawable(geom);
      geom->setUseDisplayList(false);
      // we will be drawing lines
      osg::DrawArrays* drawArrayLines = new
        osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
      geom->addPrimitiveSet(drawArrayLines);
      // create vertex array for the lines
      osg::Vec3Array* vertexData = new osg::Vec3Array();
      geom->setVertexArray(vertexData);
      osg::Vec4Array* colors = new osg::Vec4Array;

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
      // draw airspace geometry
      osg::Matrixd coorTransformation;
      osg::Vec3 coord;
      double r;
      Position arcPoint;
      // Position point;

      if (A->GetGeometrySize() > 0) {
        for (int j = 0; j < A->GetGeometrySize(); ++j) {
          // if (!A->GetPolygon(j).valid) qDebug("Polygon coords expected!");
          OpenAirspace::Geometry::GType gtype =
            A->GetGeometry().at(j)->GetGType();

          OpenAirspace::Polygon* p;
          OpenAirspace::ArcI* aa;
          OpenAirspace::ArcII* ab;
          OpenAirspace::Circle* c;
          switch (gtype) {
            // Draw polygon point
            case OpenAirspace::Geometry::DPtype:
              p = (OpenAirspace::Polygon*)A->GetGeometry().at(j);
              objectPlacer->createPlacerMatrix(p->Centre().lat,
                p->Centre().lon, 2000, coorTransformation);
              coord = osg::Vec3(0, 0, 0) * coorTransformation;
              vertexData->push_back(coord);
              colors->push_back(col);
              break;
            /* Draw a Circle - can be done in relative fashion
            case OpenAirspace::Airspace::GType::DCtype:
              OpenAirspace::Airspace::Circle* c =
                static_cast<OpenAirspace::Airspace::Circle*>(A->GetGeometry().at(i));
              for (int angle = 0; angle <= 360; ++angle) {
              }
              break;*/
            // DA radius, angleStart, angleEnd
            // add an arc, angles in degrees, radius in nm
            case OpenAirspace::Geometry::DAtype:
              aa = (OpenAirspace::ArcI*)A->GetGeometry().at(j);
              for (double angle = aa->Start()*DEG_TO_RAD;
                angle <= aa->End()*DEG_TO_RAD; angle+=ARC_GRANULARITY) {
                arcPoint = ComputeArcPoint(aa->Centre(), aa->R(), angle);
                objectPlacer->createPlacerMatrix(arcPoint.lat,
                  arcPoint.lon, 2000, coorTransformation);
                coord = osg::Vec3(0, 0, 0) * coorTransformation;
                vertexData->push_back(coord);
                colors->push_back(col);
              }
              break;
            case OpenAirspace::Geometry::DCtype:
              c = (OpenAirspace::Circle*)A->GetGeometry().at(j);
              for (double k = 0; k < 2*M_PI; k += ARC_GRANULARITY) {
                r = DistToAngle(c->R());
                arcPoint = ComputeArcPoint(c->Centre(), r, k);
                objectPlacer->createPlacerMatrix(arcPoint.lat,
                  arcPoint.lon, 2000, coorTransformation);
                coord = osg::Vec3(0, 0, 0) * coorTransformation;
                vertexData->push_back(coord);
                colors->push_back(col);
              }
              break;
            // DB coordinate1, coordinate2 : add an arc,
            // from coordinate1 to coordinate2
            case OpenAirspace::Geometry::DBtype:
              ab = (OpenAirspace::ArcII*)A->GetGeometry().at(j);
              QList<Position>* arc = new QList<Position>();
              arc->push_back(ab->Start());
              int depth = 0;
              InsertArcII(ab->Centre(), ab->Start(),
                ab->End(), ab->CW(), arc, depth);
              arc->push_back(ab->End());
              for (int k = 0; k < arc->size(); ++k) {
                objectPlacer->createPlacerMatrix(arc->at(k).lat, arc->at(k).lon,
                  2000, coorTransformation);
                coord = osg::Vec3(0, 0, 0) * coorTransformation;
                vertexData->push_back(coord);
                colors->push_back(col);
              }
              break;
          }


          // objectPlacer->createPlacerMatrix(A->GetPolygon(j).lat,
            // A->GetPolygon(j).lon, 2000, coorTransformation);
          // coord = osg::Vec3(0, 0, 0) * coorTransformation;

          // vertexData->push_back(coord);
          // colors->push_back(col);
        }
        drawArrayLines->setFirst(0);
        drawArrayLines->setCount(vertexData->size());
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
        // change the thickness of the line
        osg::LineWidth* linewidth = new osg::LineWidth();
        linewidth->setWidth(width);
        osg::StateSet* stateSet = OAGeode->getOrCreateStateSet();
        stateSet->setAttributeAndModes(linewidth,
          osg::StateAttribute::ON);
        stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        stateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
      }
    }

    QString displayName = fileName;
    int cuntSlashes = displayName.count('/');
    displayName = displayName.section('/', cuntSlashes, cuntSlashes);
    return mapLayerGroup->insertMapLayer(OAGeode, displayName);
  }
  return NULL;
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

void oaEngine::InsertArcII(const Position& centre, const Position& start,
  const Position& end, bool cw, QList<Position>* vertexList, int& depth) {
  /*if (start.lon == end.lon && start.lat == end.lat) return;
  // insert start
  // vertexList->push_back(start);
  // probably not working on the 360-0 transition
  // comp radius
  depth++;
  if (depth > 500) return;
  double dLat = start.lat - centre.lat;
  double dLon = start.lon - centre.lon;
  double r = sqrt(dLat*dLat + dLon*dLon);
  // vector of the chord
  dLat = end.lat - start.lat;
  dLon = end.lon - start.lon;
  // osg::Vec2d chord(dLat, dLon);
  // double c2 = dLat*dLat + dLon*dLon;
  double a1 = AngleRad(centre, start);
  double a2 = AngleRad(centre, end);
  double arcAngleRad = (cw) ? a2 - a1 : a1 - a2;
  if (arcAngleRad < 0) arcAngleRad += M_PI*2;
  // normal (rot 90 deg cw:(y,-x) ccw:(-y,x))
  // for arcs < 180 deg
  // TODO: correct for the circles > 180
  osg::Vec2d cN;
  if (arcAngleRad < M_PI)
    cN = (cw) ? osg::Vec2d(dLon, -dLat) : osg::Vec2d(-dLon, dLat);
  else
    cN = (!cw) ? osg::Vec2d(dLon, -dLat) : osg::Vec2d(-dLon, dLat);
  
  osg::Vec2d cent(centre.lat, centre.lon);
  double l = cN.length();
  // double a = cN.x;
  cN = osg::Vec2d(cN.x()/l, cN.y()/l);
  Position result;
  result.valid = true;
  result.lat = cent.x() + cN.x()*r;
  result.lon = cent.y() + cN.y()*r;
  if (start.lat == result.lat && start.lon == result.lon) return;
  if (end.lat == result.lat && end.lon == result.lon) return;
  if (l < 0.01) {
    vertexList->push_back(result);
  } else {
    InsertArcII(centre, start, result, cw, vertexList, depth);
    vertexList->push_back(result);
    InsertArcII(centre, result, end, cw, vertexList, depth);
  }*/
  // radius
  // double dLat = start.lat - centre.lat;
  // double dLon = (start.lon - centre.lon);
  // double r = sqrt(dLat*dLat + dLon*dLon);
  double r = DistToAngle(DistanceInMeters(start, centre)/NM_TO_M);
  // compute the angle of the arc
  double a1 = AngleRad(centre, start);
  double a2 = AngleRad(centre, end);
  Position test = ComputeArcPoint(centre, 1, -3);
  double test2 = AngleRad(centre, test);
  // double arcAngleRad = (cw) ? a2 - a1 : a1 - a2;
  // if (arcAngleRad < 0) arcAngleRad += M_2PI;
  // divide the arc to n sectors according the angle
  // int n = 2;
  // double part = arcAngleRad /n;
  /* for (int i = 1; i < n; ++i) {
    // insert the middle arc vertex
    double partAngle = (cw) ? a1 + i*part : a1 - i*part;
    Position arcPoint = ComputeArcPoint(centre, r, partAngle);
    vertexList->push_back(arcPoint);
  }*/
  /* for (double angle = ARC_GRANULARITY; angle < arcAngleRad;
    angle += ARC_GRANULARITY) {
    double partAngle = (cw) ? a1 + angle : a1 - angle;
    Position arcPoint = ComputeArcPoint(centre, r, partAngle);
    vertexList->push_back(arcPoint);
  }*/
  if (cw) {
    if (a2 < a1) a2 += M_2PI;
    for (double angle = a1; angle < a2;
      angle += ARC_GRANULARITY) {
      // if (angle > M_PI) angle -= M_2PI;
      Position arcPoint = ComputeArcPoint(centre, r, angle);
      vertexList->push_back(arcPoint);
    }
  } else {
    if (a1 < a2) a1 += M_2PI;
    for (double angle = a1; angle > a2;
      angle -= ARC_GRANULARITY) {
      // if (angle < M_PI) angle += M_2PI;
      Position arcPoint = ComputeArcPoint(centre, r, angle);
      vertexList->push_back(arcPoint);
    }
  }
  // insert end
  // vertexList->push_back(end);
}

double oaEngine::AngleRad(const Position& centre, const Position& point) {
  double lat = point.lat - centre.lat;
  double lon = (point.lon - centre.lon)*cos(point.lat*DEG_TO_RAD);
  double a;
  a = atan2(lon, lat);
  /* rubbish
  if (abs(lat) > abs(lon)) {
    a = atan(lon/lat);
    if (lat < 0) a += M_PI;
  } else {
    a = atan(-lat/lon);
    if (lon < 0) a += M_PI;
    a = a - M_PI/2;
  }*/

  /*if (lat != 0) {
    a = atan(lon/lat);
    if (lat < 0) a += M_PI;
  } else {
    // a = acot(lat/lon);
    a = (lon > 0) ? M_PI/2 : 3 * M_PI/2;
  }*/
  // if (a < 0) a += 2*M_PI;
  // if (a > 2*M_PI) a -= 2*M_PI;
  // a = a - sin %(2*M_PI);
  // if (abs(a) > M_2PI || a < 0)
    // a = a - floor(a/M_2PI)*M_2PI;
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
}  // Airspaces
}  // Updraft
