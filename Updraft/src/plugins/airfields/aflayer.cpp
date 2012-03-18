#include "aflayer.h"

namespace Updraft {

/* osg::Geode* TPLayer::createGeode(qreal scale) {
  osg::Geode* geode = new osg::Geode();
  osg::StateSet* stateSet = geode->getOrCreateStateSet();

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
  stateSet->setAttributeAndModes(new osg::BlendFunc());
  stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

  // Create and add object geometry.
  geode->addDrawable(createGeometry(scale));
  return geode;
}*/

}   // namespace Updraft
