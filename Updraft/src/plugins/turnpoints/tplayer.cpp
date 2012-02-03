#include <QtGui>
#include <osg/Geometry>
#include <osg/Billboard>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osgDB/ReadFile>
#include <osgEarthUtil/ObjectPlacer>
#include "tplayer.h"

namespace Updraft {

osg::Geometry* TPLayer::createGeometry(qreal scale) {
  osg::Geometry* geometry = new osg::Geometry();

  osg::Vec3Array* vertices = new osg::Vec3Array(4);
  (*vertices)[0] = osg::Vec3(-scale, 0.0, -scale);
  (*vertices)[1] = osg::Vec3( scale, 0.0, -scale);
  (*vertices)[2] = osg::Vec3( scale, 0.0, scale);
  (*vertices)[3] = osg::Vec3(-scale, 0.0, scale);
  geometry->setVertexArray(vertices);

  osg::Vec4Array* colors = new osg::Vec4Array(1);
  (*colors)[0] = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
  geometry->setColorArray(colors);
  geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

  osg::Vec3Array* normals = new osg::Vec3Array(1);
  (*normals)[0] = osg::Vec3(0.0f, -1.0f, 0.0f);
  geometry->setNormalArray(normals);
  geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

  osg::Vec2Array* texCoords = new osg::Vec2Array(4);
  (*texCoords)[0] = osg::Vec2(0.0, 0.0);
  (*texCoords)[1] = osg::Vec2(1.0, 0.0);
  (*texCoords)[2] = osg::Vec2(1.0, 1.0);
  (*texCoords)[3] = osg::Vec2(0.0, 1.0);
  geometry->setTexCoordArray(0, texCoords);

  geometry->addPrimitiveSet(
    new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
  return geometry;
}

TPLayer::TPLayer(bool displayed_, osgEarth::Util::ObjectPlacer* objectPlacer_,
  const TPFile *file_, const QString &dataDir)
  : billboard(new osg::Billboard()), objectPlacer(objectPlacer_),
  file(file_), displayed(displayed_) {
  if (billboard == NULL || objectPlacer == NULL || file == NULL) {
    return;
  }

  // Set mode of the billboard to always look towards camera.
  billboard->setMode(osg::Billboard::POINT_ROT_EYE);
  osg::StateSet* stateSet = billboard->getOrCreateStateSet();

  // Create texture
  QString texPath = dataDir + "/turnpoint.tga";
  osg::Image *image = osgDB::readImageFile(texPath.toStdString());
  osg::Texture2D *texture = new osg::Texture2D();
  texture->setImage(image);

  // Turn off lighting for the billboard.
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  // Turn on texturing, bind texture.
  stateSet->setTextureAttributeAndModes(0, texture);

  // Turn on blending.
  stateSet->setAttributeAndModes(new osg::BlendFunc(),
    osg::StateAttribute::ON);

  // Create geometry for turn-point billboards.
  osg::Geometry* geometry = createGeometry(2000.0);

  TTPList points = file->getTurnPoints();

  for (TTPList::const_iterator itPoint = points.begin();
    itPoint != points.end(); ++itPoint) {
    osg::Matrixd matrix;

    // Turn-point object is placed 2000 meters above it's position (terrain).
    if (!objectPlacer->createPlacerMatrix(itPoint->location.lat,
      itPoint->location.lon, itPoint->location.alt + 2000.0, matrix)) {
      continue;
    }

    billboard->addDrawable(geometry, osg::Vec3() * matrix);
  }
}

TPLayer::~TPLayer() {
  delete file;
}

osg::Node* TPLayer::getNode() const {
  return billboard;
}

bool TPLayer::isDisplayed() {
  return displayed;
}

void TPLayer::display(bool displayed_) {
  displayed = displayed_;
}

}  // End namespace Updraft
