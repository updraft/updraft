#include <QtGui>
#include <osg/Billboard>
#include <osg/AutoTransform>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osgDB/ReadFile>
#include <osgEarthUtil/ObjectPlacer>
#include <osgText/Text>
#include "tplayer.h"
#include "coreinterface.h"
#include "turnpoints.h"
#include "mapobject.h"

namespace Updraft {

osg::Geometry* TPLayer::createGeometry(qreal scale) {
  osg::Geometry* geometry = new osg::Geometry();

  osg::Vec3Array* vertices = new osg::Vec3Array(4);
  (*vertices)[0] = osg::Vec3(-scale, -scale, 0.0);
  (*vertices)[1] = osg::Vec3( scale, -scale, 0.0);
  (*vertices)[2] = osg::Vec3( scale, scale, 0.0);
  (*vertices)[3] = osg::Vec3(-scale, scale, 0.0);
  geometry->setVertexArray(vertices);

  osg::Vec4Array* colors = new osg::Vec4Array(1);
  (*colors)[0] = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
  geometry->setColorArray(colors);
  geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

  osg::Vec3Array* normals = new osg::Vec3Array(1);
  (*normals)[0] = osg::Vec3(0.0f, 0.0f, 1.0f);
  geometry->setNormalArray(normals);
  geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

  osg::Vec2Array* texCoords = new osg::Vec2Array(4);
  (*texCoords)[0] = osg::Vec2(0.0, 0.0);
  (*texCoords)[1] = osg::Vec2(1.0, 0.0);
  (*texCoords)[2] = osg::Vec2(1.0, 1.0);
  (*texCoords)[3] = osg::Vec2(0.0, 1.0);
  geometry->setTexCoordArray(0, texCoords);

  geometry->addPrimitiveSet(new osg::DrawArrays(
    osg::PrimitiveSet::QUADS, 0, 4));
  return geometry;
}

osg::Geode* TPLayer::createGeode(qreal scale, bool isAirfield) {
  osg::Geode* geode = new osg::Geode();
  osg::StateSet* stateSet = geode->getOrCreateStateSet();

  // Create texture
  QString imageName("");
  if (isAirfield) {
    imageName = "airfield.tga";
  } else {
    imageName = "turnpoint.tga";
  }
  QString texPath = dataDir + "/" + imageName;
  osg::Image *image = osgDB::readImageFile(texPath.toStdString());
  osg::Texture2D *texture = new osg::Texture2D();
  texture->setImage(image);

  // Turn off lighting for the billboard.
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  // Turn on texturing, bind texture.
  stateSet->setTextureAttributeAndModes(0, texture);

  // Turn on blending.
  stateSet->setAttributeAndModes(new osg::BlendFunc());
  stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

  // Create and add object geometry.
  geode->addDrawable(createGeometry(scale));
  return geode;
}

osg::AutoTransform* TPLayer::createAutoTransform(const osg::Matrix& matrix,
  osg::Geode* geode) {
  osg::AutoTransform* at = new osg::AutoTransform();
  at->addChild(geode);

  at->setAutoRotateMode(osg::AutoTransform::NO_ROTATION);
  at->setAutoScaleToScreen(true);

  // Autoscaling stop during zooming in when the object is minScale * usualScale
  // or during zooming out when the object is maxScale * usualScale.
  // usualScale is the scale of normal zooming.
  // Size of the object when the zoom is larger than during autoScale
  //  remains size*minScale.
  // Size of the object when the zoom is less than during autoScale
  //  remains size*maxScale.
  at->setMinimumScale(10.0);
  at->setMaximumScale(100.0);

  at->setRotation(matrix.getRotate());
  at->setPosition(matrix.getTrans());
  return at;
}

osg::Node* TPLayer::createAutoScale(
  const osg::Vec3& position,
  qreal characterSize,
  const QString& message,
  qreal minScale,
  qreal maxScale) {
  // Creates the autoscale transform node
  QString fontName = "AmphionExtrabold Regular.ttf";
  QString fontPath = dataDir + "/fonts/" + fontName;

  // set the osgText
  osgText::Text* text = new osgText::Text;
  text->setCharacterSize(static_cast<float>(characterSize));
  text->setText(message.toStdString());
  text->setFont(fontPath.toStdString());
  text->setAlignment(osgText::Text::LEFT_BASE_LINE);
  text->setColor(labelColour);

  // define the geode
  osg::Geode* geode = new osg::Geode;
  geode->addDrawable(text);
  geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  // Create the autotransform
  osg::AutoTransform* at = new osg::AutoTransform;
  at->addChild(geode);

  at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
  at->setAutoScaleToScreen(true);
  at->setMinimumScale(static_cast<double>(minScale));
  at->setMaximumScale(static_cast<double>(maxScale));
  at->setPosition(position);

  return at;
}

TPLayer::TPLayer(bool displayed_, osgEarth::Util::ObjectPlacer* objectPlacer_,
  const TPFile *file_, const QString &dataDir_, TurnPoints* parent_)
  : group(new osg::Group()), objectPlacer(objectPlacer_),
  file(file_), displayed(displayed_), dataDir(dataDir_), parent(parent_) {
  if (group == NULL || objectPlacer == NULL || file == NULL) {
    return;
  }

  // Settings
  labelColour   = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
  labelMaxScale = 20;

  // Create node for one turn-point.
  // It will be shared among Autotransform nodes.
  osg::Geode *geodeTp = createGeode(25.0, 0);
  osg::Geode *geodeAf = createGeode(25.0, 1);

  TTPList points = file->getTurnPoints();


  for (TTPList::const_iterator itPoint = points.begin();
    itPoint != points.end(); ++itPoint) {
    osg::Matrixd matrix;

    // Add little random displacement to altitude.
    // Reason: If two overlapping objects are in the same height,
    // the result of blending is ugly.
    double d = 0.0001 * (qrand() % 10000);

    // Create matrix from TP's position.
    // Turn-point is placed 100 meters above it's position (terrain).
    if (!objectPlacer->createPlacerMatrix(itPoint->location.lat,
      itPoint->location.lon, itPoint->location.alt + 100.0 + d, matrix)) {
      continue;
    }

    // Create new Autotransform node.
    osg::AutoTransform* tpNode = NULL;
    if (itPoint->type >= 2 && itPoint->type <= 5) {
      qreal afAngle = itPoint->rwyHeading * 3.14 / 180;
      matrix = osg::Matrixd::identity().rotate
        (afAngle, osg::Vec3f(0.0f, 0.0f, -1.0f)) * matrix;
      tpNode = createAutoTransform(matrix, geodeAf);
    } else {
      tpNode = createAutoTransform(matrix, geodeTp);
    }

    // Make the autotransform node pickable
    TPMapObject* mapObject = new TPMapObject(itPoint->name);
    mapObjects.push_back(mapObject);
    parent->getCoreInterface()->registerOsgNode(tpNode, mapObject);

    group->addChild(tpNode);

    group->addChild(createAutoScale(
      matrix.getTrans(),
      20.0,
      itPoint->name,
      0.0,
      20.0));
  }
}

TPLayer::~TPLayer() {
  foreach(TPMapObject* tpObj, mapObjects) {
    delete tpObj;
  }
  delete file;
}

osg::Node* TPLayer::getNode() const {
  return group;
}

bool TPLayer::isDisplayed() {
  return displayed;
}

void TPLayer::display(bool displayed_) {
  displayed = displayed_;
}

}  // End namespace Updraft
