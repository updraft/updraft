#include "airspaces.h"

// #include <osgDB/ReadFile>
// #include <osg/LineWidth>
// #include <osgEarthUtil/ObjectPlacer>
// #include <osg/PositionAttitudeTransform>
#include <osg/Geometry>

#include "../../libraries/openairspace/openairspace.h"


namespace Updraft {
namespace Core {

Airspaces::Airspaces() {
  OAirspaceFileReg.category = CATEGORY_PERSISTENT;
  OAirspaceFileReg.extension = ".txt";
  OAirspaceFileReg.typeDescription = tr("Open Airspace file");
  OAirspaceFileReg.roleDescription = tr("Import Airspace");
  OAirspaceFileReg.importDirectory = "airspaces";
  OAirspaceFileReg.roleId = 0;
  OAirspaceFileReg.plugin = this;
}

QString Airspaces::getName() {
  return QString("airspaces");
}

unsigned Airspaces::getPriority() {
  return 0;  // TODO(cestmir): decide on the priority of plugins
}

void Airspaces::initialize() {
  qDebug("Loading deafault for testing");
  OpenAirspace::Parser P("c:/Updraft/CZ2011.txt");

  // Create map layers items in the left pane.
  mapLayerGroup = core->createMapLayerGroup("Airspaces");
  if (mapLayerGroup != NULL) {
    // create map placer: to draw in the map
    osgEarth::Util::ObjectPlacer* objectPlacer =
      mapLayerGroup->getObjectPlacer();

    // LAYER #1
    // draw some lines in Czech Republic
    osg::Geode* geode = new osg::Geode();  // the root of the subscene
    osg::Geometry* geom = new osg::Geometry();
    geode->addDrawable(geom);
    geom->setUseDisplayList(false);
    // we will be drawing lines
    osg::DrawArrays* drawArrayLines = new
      osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    geom->addPrimitiveSet(drawArrayLines);
    // create vertex array for the lines
    osg::Vec3Array* vertexData = new osg::Vec3Array();
    geom->setVertexArray(vertexData);

    // add vertices into the vertex array
    vertexData->push_back(osg::Vec3(0, 0, 0));
    vertexData->push_back(osg::Vec3(0, 0, 2000));
    vertexData->push_back(osg::Vec3(100000, 0, 4000));
    vertexData->push_back(osg::Vec3(100000, -100000, 4000));
    vertexData->push_back(osg::Vec3(0, 0, 0));

    // set color array: one color for each vertex
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));  // index 0 red
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));  // index 1 green
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));  // index 2 blue
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));  // index 3 white
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));  // index 4 red

    geom->setColorArray(colors);
    // set the colors to be one color per one vertex
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    // set where to start the line strip and how long it will be
    drawArrayLines->setFirst(0);
    drawArrayLines->setCount(vertexData->size());

    // create the thickness of the line object
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(2.0f);

    // set the thickness to the scene root
    geode->getOrCreateStateSet()->setAttributeAndModes(linewidth,
      osg::StateAttribute::ON);

    // move the center of the node to Prague
    osg::Node* randomLines = objectPlacer->placeNode(geode,
      50.087811, 14.42046, 1000);

    Updraft::MapLayerInterface* layer1 =
      mapLayerGroup->insertMapLayer(randomLines, "Relative Lines", 0);
    layer1->connectSignalDisplayed
      (this, SLOT(mapLayerDisplayed(bool, MapLayerInterface*)));
    mapLayers.append(layer1);
*/
  qDebug("airspaces laoded");
}

void Airspaces::deinitialize() {
  // delete mapLayerGroup;
  qDebug("airspaces unloaded");
}

Q_EXPORT_PLUGIN2(airspaces, Airspaces)

}  // End namespace Core
}  // End namespace Updraft

