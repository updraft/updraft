#include "oaengine.h"
#include "../../core/maplayer.h"

namespace Updraft {
namespace Airspaces {

  // Definition of global pointer to coreinterface.
  // CoreInterface *g_core = NULL;

oaEngine::oaEngine(MapLayerGroupInterface* LG,
  CoreInterface* g_core) {
  this->mapLayerGroup = LG;

  // some defaults
  // Turn this on to compute the terrain elevation
  // in every polygon vertex
  USE_POINTWISE_ELEVATION = true;
  // Turn this on to draw the airspace polygons to lvl 0
  DRAW_UNDERGROUND        = false;
  // Turn this on to draw the selected face of the polygon (NA)
  TOP_FACE                = false;
  BOTTOM_FACE             = false;
  SIDE_FACE               = true;
  // Turn this on to draw the polygon wireframe
  TOP_WIREFRAME           = true;
  BOTTOM_WIREFRAME        = true;
  SIDE_WIREFRAME          = false;
  // Turn this on to draw the side with gradient
  SIDE_COL_GRADIENT       = true;
  // Opacity settings
  POLY_OPACITY_BOTTOM     = 0.5;
  POLY_OPACITY_TOP        = 0.1;
  WIRE_OPACITY_BOTTOM     = 0.5;  // 0.6;
  WIRE_OPACITY_TOP        = 0.5;  // 0.2;
  // Elevation resolution setting
  ELEV_TILE_RESOLUTION    = 0.5;  // 0.01 - slow, 0.1 fast
  // Sets the default height of ground and ceiling
  GND                     = 0;
  ROOF                    = 80000;

  // init
  heightRefPoint  = NULL;
  mapLayers       = NULL;
  OAGeode         = NULL;

  // Init the elevation manager
  // osgEarth::Map* map = mapLayerGroup->getMapNode()->getMap();
  // elevationMan = new osgEarth::Util::ElevationManager(map);
  elevationMan = g_core->getElevationManager();
}

QVector<MapLayerInterface*>* oaEngine::DrawII(const QString& fileName) {
  if (!Draw(fileName))
    return NULL;

  QVector<MapLayerInterface*>* layers = new QVector<MapLayerInterface*>();
  for (int i = 0; i < mapLayers->size(); ++i) {
    layers->push_back(mapLayerGroup->createMapLayer(
      mapLayers->at(i).first, mapLayers->at(i).second));
  }

  return layers;
}

QVector<QPair<osg::Node*, QString> >* oaEngine::Draw(const QString& fileName) {
  // if valid maplayer proceed
  if (mapLayerGroup != NULL) {
    // Parse the file
    OpenAirspace::Parser AirspaceSet(fileName);
    if (!AirspaceSet.size()) return NULL;

    // reset const
    heightRefPoint = NULL;
    mapLayers = NULL;
    OAGeode = NULL;

    // set the defeault line width
    this->width = 1.0f;

    // set the default line colour
    col = osg::Vec4f(0.0f, 0.5f, 1.0f, DEFAULT_TRANSPARENCY);


    // get name to display
    QString displayName = fileName.left(fileName.indexOf('.'));
    int cuntSlashes = displayName.count('/');
    displayName = displayName.section('/', cuntSlashes, cuntSlashes);

    // currently used name suffix
    QString nameSuffix("");


    // Cycle through all the parsed airspaces
    // and draw the geometry
    for (size_t i = 0; i < AirspaceSet.size(); ++i) {
      // Process the Airspace
      OpenAirspace::Airspace* A = AirspaceSet.at(i);

      // get the bundle of airspaces with the same name/class
      QString aName = QString(A->GetClassName());
        // (A->GetName() == NULL) ?
        // QString("N/A") : (*A->GetName());
      if (nameSuffix != aName) {  // A->GetClassName()
        // if there is a geode initialized
        // insert new into the array of layers
        // or find one of the same name
        if (OAGeode)
          PushLayer(OAGeode, nameSuffix);

        // change the suffix to current one
        nameSuffix = aName;

        // try to find out if exists
        OAGeode = FindLayer(nameSuffix);

        // if not found init the subscene
        if (!OAGeode)
          OAGeode = new osg::Geode();
      }

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
      if (!DRAW_UNDERGROUND) {
        if (floor == 0) floorAgl = true;
      }

      // To destroy artefacts of two planes in one space
      double rnd = 0.05 * (qrand() % 100);
      floor += rnd;
      ceiling -= rnd;

      // array of coords to draw
      QVector<Position>* pointsWGS = new QVector<Position>();

      // cycle through the geometry group
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
              if (!heightRefPoint)
                heightRefPoint = new Position(aa->Centre());
          } else if (gtype == OpenAirspace::Geometry::DCtype) {
              OpenAirspace::Circle* c =
                (OpenAirspace::Circle*)A->GetGeometry().at(j);
              InsertCircle(*c, pointsWGS);
              if (!heightRefPoint)
                heightRefPoint = new Position(c->Centre());
          } else if (gtype == OpenAirspace::Geometry::DBtype) {
            OpenAirspace::ArcII* ab =
              (OpenAirspace::ArcII*)A->GetGeometry().at(j);
            InsertArcII(*ab, pointsWGS);
            if (!heightRefPoint)
                heightRefPoint = new Position(ab->Centre());
          }
        }

        // close the polygon if open :
        if (pointsWGS) {
          if ((pointsWGS->first().lat != pointsWGS->last().lat) ||
            (pointsWGS->first().lon != pointsWGS->last().lon)) {
            // qDebug("otevreny");
            pointsWGS->push_back(pointsWGS->first());
          }
        }

        // Compute the height data
        QVector<double>* pointsGnd = ComputeHeightData(
          &floor, &ceiling, &floorAgl, &ceilingAgl,
          heightRefPoint, A, pointsWGS);

        // Draw the geometry into the OpenGl Array
        FillOGLArrays(pointsWGS, pointsGnd, floor, ceiling,
          floorAgl, ceilingAgl);

        delete pointsWGS;
        pointsWGS = NULL;

        delete heightRefPoint;
        heightRefPoint = NULL;

        if (pointsGnd) {
          delete pointsGnd;
          pointsGnd = NULL;
        }
      }  // cycle through geometry
    }   // cycle through airspaces

    if (OAGeode && OAGeode->getNumDrawables()) {
      // insert the created layer
      // PushLayer(OAGeode, displayName + nameSuffix);
      PushLayer(OAGeode, nameSuffix);
    }
    // return mapLayerGroup->insertMapLayer(OAGeode, displayName);
    return mapLayers;
  }
  return NULL;
}

QVector<double>* oaEngine::ComputeHeightData(
  int* floor, int* ceiling,
  bool* floorAgl, bool* ceilingAgl,
  Position* heightRefPoint,
  OpenAirspace::Airspace* A,
  QVector<Position>* pointsWGS) {
  // Compute the ground level
  QVector<double>* pointsGnd = NULL;
  if (*floorAgl || *ceilingAgl) {
    if (!heightRefPoint && A->GetGeometrySize() > 0) {
      // compute the center of gravity
      double sumLon = 0;
      double sumLat = 0;
      for (int m = 0; m < A->GetGeometrySize(); ++m) {
        sumLon += A->GetGeometry().at(m)->Centre().lon;
        sumLat += A->GetGeometry().at(m)->Centre().lat;
      }
      sumLat /= A->GetGeometrySize();
      sumLon /= A->GetGeometrySize();
      heightRefPoint = new Position();
      heightRefPoint->lat = sumLat;
      heightRefPoint->lon = sumLon;
      heightRefPoint->valid = true;
    }

    // use only one refpoint
    double res = 0;
    double addGnd;

    // query the elevation layer for the elev data
    elevationMan->getElevation(
        heightRefPoint->lon,
        heightRefPoint->lat,
        ELEV_TILE_RESOLUTION, 0,
        addGnd, res);
    if (addGnd < 0)
      addGnd = 0;

    // Get the elevation data for whole airspace
    // or for each and every point of the polygon
    if (USE_POINTWISE_ELEVATION) {
      pointsGnd = new QVector<double>();
      for (int k = 0; k < pointsWGS->size(); ++k) {
        elevationMan->getElevation(
          pointsWGS->at(k).lon,
          pointsWGS->at(k).lat,
          ELEV_TILE_RESOLUTION, 0, addGnd, res);
        if (addGnd < 0)
          addGnd = 0;
        pointsGnd->push_back(addGnd);
      }
    } else {
      if (*floorAgl) *floor += addGnd * M_TO_FT;
      if (*ceilingAgl) *ceiling += addGnd * M_TO_FT;
    }
  }
  return pointsGnd;
}

void oaEngine::FillOGLArrays(
  QVector<Position>* pointsWGS,
  QVector<double>* pointsGnd,
  int floor, int ceiling,
  bool floorAgl, bool ceilingAgl) {
  // get the polygon orientation if needed;
  bool cw = true;
  if (TOP_FACE || BOTTOM_FACE)
    cw = IsPolyOrientationCW(pointsWGS);

  // OGL draw the geom
  osg::Geometry* geom;

  // Draw volume
  osg::Vec4
    faceTopCol(col.x(), col.y(), col.z(), POLY_OPACITY_TOP);
  osg::Vec4
    faceBottomCol(col.x(), col.y(), col.z(), POLY_OPACITY_BOTTOM);
  // draw top polygon
  if (TOP_FACE) {
    geom = DrawPolygon(pointsWGS, pointsGnd, faceTopCol,
      osg::PrimitiveSet::POLYGON, ceiling, ceilingAgl, cw);
    OAGeode->addDrawable(geom);
  }
  // draw bottom poly
  if (BOTTOM_FACE && floor > GND) {
    geom = DrawPolygon(pointsWGS, pointsGnd, faceBottomCol,
      osg::PrimitiveSet::POLYGON, floor, floorAgl, cw);
    OAGeode->addDrawable(geom);
  }
  // draw the sides
  if (SIDE_FACE) {
    geom = DrawPolygonSides(pointsWGS, pointsGnd, faceBottomCol,
      faceTopCol, osg::PrimitiveSet::TRIANGLE_STRIP, floor, ceiling,
      floorAgl, ceilingAgl);
    OAGeode->addDrawable(geom);
  }

  // Draw contours
  osg::Vec4
    wireTopCol(col.x(), col.y(), col.z(), WIRE_OPACITY_TOP);
  osg::Vec4
    wireBottomCol(col.x(), col.y(), col.z(), WIRE_OPACITY_BOTTOM);
  // draw top polygon
  if (TOP_WIREFRAME) {
    geom = DrawPolygon(pointsWGS, pointsGnd, wireTopCol,
      osg::PrimitiveSet::LINE_STRIP, ceiling, ceilingAgl, cw);
    OAGeode->addDrawable(geom);
  }
  // draw bottom poly
  if (BOTTOM_WIREFRAME && floor > GND) {
    geom = DrawPolygon(pointsWGS, pointsGnd, wireBottomCol,
      osg::PrimitiveSet::LINE_STRIP, floor, floorAgl, cw);
    OAGeode->addDrawable(geom);
  }
  // Side wireframe
  if (SIDE_WIREFRAME) {
    geom = DrawPolygonSides(pointsWGS, pointsGnd, wireBottomCol,
      wireTopCol, osg::PrimitiveSet::LINES, floor, ceiling,
      floorAgl, ceilingAgl);
    OAGeode->addDrawable(geom);
  }
}

osg::Geode* oaEngine::FindLayer(const QString& name) {
  // find, remove from array and return the Node with name
  osg::Geode* result = NULL;
  if (!mapLayers) return NULL;
  QVector<QPair<osg::Node*, QString> >::iterator it;
  // it = mapLayers->begin();
  for (it = mapLayers->begin(); it < mapLayers->end(); ++it) {
    if ((*it).second == name) {
      result = (osg::Geode*)(*it).first;
      mapLayers->erase(it);
    }
  }
  return result;
}

void oaEngine::PushLayer(osg::Geode* OAGeode, const QString& displayName) {
  // change the thickness of the line
  osg::LineWidth* linewidth = new osg::LineWidth();
  linewidth->setWidth(width);

  // set geode params
  osg::StateSet* stateSet = OAGeode->getOrCreateStateSet();
  stateSet->setAttributeAndModes(linewidth,
    osg::StateAttribute::ON);
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

  // Enable blending, select transparent bin.
  stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
  stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

  // Enable depth test so that an opaque polygon will occlude
  // a transparent one behind it.
  stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

  // Conversely, disable writing to depth buffer so that
  // a transparent polygon will allow polygons behind it to shine thru.
  // OSG renders transparent polygons after opaque ones.
  osg::Depth* depth = new osg::Depth;
  depth->setWriteMask(false);
  stateSet->setAttributeAndModes(depth, osg::StateAttribute::ON);

  // Disable conflicting modes.
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  // result in new map layer
  if (!mapLayers)
    mapLayers = new QVector<QPair<osg::Node*, QString> >();
  // MapLayerInterface* newLayer = new Core::MapLayer((osg::Node*)OAGeode);
  // newLayer->setVisible(false);
  // QPair<osg::Node*, QString> toPush

  // To sort the airspaces find where to insert
  QVector<QPair<osg::Node*, QString> >::iterator it;
  it = mapLayers->begin();
  while (it < mapLayers->end() && (*it).second < displayName)
    ++it;

  // mapLayers->push_back(QPair<osg::Node*, QString>(OAGeode, displayName));
  mapLayers->insert(it, QPair<osg::Node*, QString>(OAGeode, displayName));
}

osg::Geometry* oaEngine::DrawPolygon(
  QVector<Position>* pointsWGS,
  const QVector<double>* pointsGnd,
  osg::Vec4& col,
  osg::PrimitiveSet::Mode primitiveMode,
  int height,
  bool agl, bool cw) {
  // Draw the closed polygon. In case of filled polygon,
  // ensure of its filled correctly (it's TRIANGLE_FAN)
  // that means recursively fill all the correctly oriented points

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
  // if polygon face add the center
  /* if (heightRefPoint &&
    primitiveMode == osg::PrimitiveSet::POLYGON) {
    if (pointsGnd) addGnd = pointsGnd->at(0);
    objectPlacer->createPlacerMatrix(
      heightRefPoint->lat,
      heightRefPoint->lon,
      (height*FT_TO_M) + addGnd,
      coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;
    vertexData->push_back(coord);
  }*/
  // fill the array
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

osg::Geometry* oaEngine::DrawPolygonSides(const QVector<Position>* pointsWGS,
  const QVector<double>* pointsGnd,
  osg::Vec4& colBot, osg::Vec4& colTop,
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
    if (SIDE_COL_GRADIENT)
      colors->push_back(colTop);

    if (floorAgl && pointsGnd
      && pointsGnd->size() == pointsWGS->size()) {
      addFloorGnd = pointsGnd->at(k);
    }
    objectPlacer->createPlacerMatrix(pointsWGS->at(k).lat,
      pointsWGS->at(k).lon, floor*FT_TO_M + addFloorGnd,
      coorTransformation);
    coord = osg::Vec3(0, 0, 0) * coorTransformation;

    vertexData->push_back(coord);
    if (SIDE_COL_GRADIENT)
      colors->push_back(colBot);
    else if (primitiveMode == osg::PrimitiveSet::LINES)
      colors->push_back(colBot);
  }
  if (!SIDE_COL_GRADIENT && !primitiveMode == osg::PrimitiveSet::LINES)
    colors->push_back(colBot);

  drawArrayLines->setFirst(0);
  drawArrayLines->setCount(vertexData->size());
  geom->setColorArray(colors);
  if (SIDE_COL_GRADIENT)
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
  else
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
  QVector<Position>* vertexList) {
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
  QVector<Position>* vertexList) {
  // init:
  const Position centre = ab.Centre();
  const Position start = ab.Start();
  const Position end = ab.End();
  const bool cw = ab.CW();

  // compute the radius as mean of two distances (start/end to centre)
  double r = (DistToAngle(DistanceInMeters(start, centre) * M_TO_NM)
    + DistToAngle(DistanceInMeters(end, centre) * M_TO_NM)) * 0.5;

  // compute the angle of the arc
  double a1 = AngleRad(centre, start);
  double a2 = AngleRad(centre, end);

  // insert arc points coordinates
  // insert start
  // vertexList->push_back(ab.Start());
  vertexList->push_back(ComputeArcPoint(centre, r, a1));

  // mid points
  InsertMidArc(centre, a1, a2, cw, r, vertexList);

  // insert end
  // vertexList->push_back(ab.End());
  vertexList->push_back(ComputeArcPoint(centre, r, a2));
}

void oaEngine::InsertCircle(const OpenAirspace::Circle &cc,
  QVector<Position>* vertexList) {
  const double r = DistToAngle(cc.R());
  for (double k = 0; k < M_2PI; k += ARC_GRANULARITY) {
    vertexList->push_back(ComputeArcPoint(cc.Centre(), r, k));
  }
  // close the loop
  vertexList->push_back(ComputeArcPoint(cc.Centre(), r, 0));
}

void oaEngine::InsertMidArc(const Position& centre, double from,
  double to, const bool cw, const double& r,
  QVector<Position>* vertexList) {
  if (cw) {
    if (to < from) to += M_2PI;
    for (double angle = from + ARC_GRANULARITY; angle < to;
      angle += ARC_GRANULARITY) {
      Position arcPoint = ComputeArcPoint(centre, r, angle);
      vertexList->push_back(arcPoint);
    }
  } else {
    if (from < to) from += M_2PI;
    for (double angle = from - ARC_GRANULARITY; angle > to;
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

bool oaEngine::IsPolyOrientationCW(QVector<Position>* pointsWGS) {
  // TODO(monkey): finish this
  return true;
  // if not enough geometry
  if (!pointsWGS || pointsWGS->size() < 2)
    return true;
  // compute the first angle
  Position first = pointsWGS->at(0);
  Position second = pointsWGS->at(1);
  double alpha1 = AngleRad(first, second);
  double sum = 0;
  double prev = alpha1;
  for (int i = 2; i < pointsWGS->size(); ++i) {
    first = second;
    second = pointsWGS->at(i);
    double alpha = AngleRad(first, second);
    double add = alpha - prev;
    if (add > M_PI)
      add = -M_2PI + add;
    else if (add < -M_PI)
      add = M_2PI + add;
    sum += add;
    prev = alpha;
  }
  double add = alpha1 - prev;
  if (add > M_PI)
    add = -M_2PI + add;
  else if (add < -M_PI)
    add = M_2PI + add;
  sum += add;
  // if (abs(sum) == 0 || abs(sum) > 7)
    // qDebug("spatne");
  // qDebug("%f", sum);
  return (sum > 0) ? true : false;
}

double oaEngine::AngleRadPos(const Position& centre, const Position& point) {
  double angle = AngleRad(centre, point);
  return (angle >= 0) ? angle : angle + M_2PI;
}
}  // Airspaces
}  // Updraft
