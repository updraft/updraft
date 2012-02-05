#include "oaengine.h"

namespace Updraft {
namespace Airspaces {

MapLayerInterface* oaEngine::Draw(const QString& fileName)
{
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
      OpenAirspace::Airspace A = AirspaceSet.at(i);
      if (A.GetBrush()) {
        float r = A.GetBrush()->R/255.0f;
        float g = A.GetBrush()->G/255.0f;
        float b = A.GetBrush()->B/255.0f;
        if ( r >= 0 && g >= 0 && b >= 0) 
          col.set(r, g, b, col.w());
      }
      if (A.GetPen()) {
        float r = A.GetPen()->R/255.0f;
        float g = A.GetPen()->G/255.0f;
        float b = A.GetPen()->B/255.0f;
        if ( r >= 0 && g >= 0 && b >= 0)
          col.set(r, g, b, col.w());
        width = A.GetPen()->width;
      }
      // draw airspace geometry
      osg::Matrixd coorTransformation;
      osg::Vec3 coord;

      for (int j = 0; j < A.GetGeometry().size(); ++j) {
        // if (!A.GetPolygon(j).valid) qDebug("Polygon coords expected!");
        OpenAirspace::Airspace::GType gtype = static_cast
          <OpenAirspace::Airspace::Poly*>(A.GetGeometry().at(i))->type;

        switch (gtype) {
          // Draw polygon point
          case OpenAirspace::Airspace::GType::DPtype:
            OpenAirspace::Airspace::Poly* p =
              static_cast<OpenAirspace::Airspace::Poly*>(A.GetGeometry().at(i));
            objectPlacer->createPlacerMatrix(p->coor->lat,
              p->coor->lon, 2000, coorTransformation);
            coord = osg::Vec3(0, 0, 0) * coorTransformation;
            vertexData->push_back(coord);
            colors->push_back(col);
            break;
          // Draw a Circle
          case OpenAirspace::Airspace::GType::DCtype:
            OpenAirspace::Airspace::Circle* c =
              static_cast<OpenAirspace::Airspace::Circle*>(A.GetGeometry().at(i));
            int r = c->
            for (int angle = 0; angle <= 360; ++angle) {
            }
            break;
        }


        objectPlacer->createPlacerMatrix(A.GetPolygon(j).lat,
          A.GetPolygon(j).lon, 2000, coorTransformation);
        coord = osg::Vec3(0, 0, 0) * coorTransformation;

        vertexData->push_back(coord);
        colors->push_back(col);
      }
      drawArrayLines->setFirst(0);
      drawArrayLines->setCount(vertexData->size());
      geom->setColorArray(colors);
      geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
      // change the thickness of the line
      osg::LineWidth* linewidth = new osg::LineWidth();
      linewidth->setWidth(width);
      OAGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth,
        osg::StateAttribute::ON);
    }

    QString displayName = fileName;
    int cuntSlashes = displayName.count('/');
    displayName = displayName.section('/',cuntSlashes, cuntSlashes);
    return mapLayerGroup->insertMapLayer(OAGeode, displayName);
  }
  return NULL;
}

double oaEngine::DistToAngle(double dInNauticalMiles) {
  double dInMeters = 1852*dInNauticalMiles;
  double angleRad = asin(dInMeters/EARTH_RADIUS_IN_METERS);
  double angleDeg = angleRad/M_PI * 180;
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

}  // Airspaces
}  // Updraft

