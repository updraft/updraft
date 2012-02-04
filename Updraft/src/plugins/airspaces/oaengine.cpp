#include "oaengine.h"


#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

#include "../../libraries/openairspace/openairspace.h"

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
        osg::DrawArrays(osg::PrimitiveSet::POLYGON);
      geom->addPrimitiveSet(drawArrayLines);
      // create vertex array for the lines
      osg::Vec3Array* vertexData = new osg::Vec3Array();
      geom->setVertexArray(vertexData);
      osg::Vec4Array* colors = new osg::Vec4Array;

      // set colour
      OpenAirspace::Airspace A = AirspaceSet.at(i);
      if (A.GetBrush().valid) {
        float r = A.GetBrush().R/255.0f;
        float g = A.GetBrush().G/255.0f;
        float b = A.GetBrush().B/255.0f;
        col.set(r, g, b, col.w());
      }
      if (A.GetPen().valid) {
        float r = A.GetPen().R/255.0f;
        float g = A.GetPen().G/255.0f;
        float b = A.GetPen().B/255.0f;
        col.set(r, g, b, col.w());
        width = A.GetPen().width;
      }
      // draw polygons
      osg::Matrixd coorTransformation;
      osg::Vec3 coord;
      for (int j = 0; j < A.GetPolygon().size(); ++j) {
        if (!A.GetPolygon(j).valid) qDebug("Polygon coords expected!");
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

    return mapLayerGroup->insertMapLayer(OAGeode, fileName);
  }

}
}  // Airspaces
}  // Updraft

