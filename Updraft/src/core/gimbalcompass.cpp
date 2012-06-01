#include "gimbalcompass.h"

#include <osgDB/ReadFile>
#include <osg/AutoTransform>

#include "updraft.h"

namespace Updraft {
namespace Core {

GimbalCompass::GimbalCompass() {
  osg::Node* testCube = NULL;
  testCube = osgDB::readNodeFile(
    (updraft->getDataDirectory()+"/test.obj").toStdString());

  osg::AutoTransform* compassTransform = new osg::AutoTransform();

  this->addChild(compassTransform);
  compassTransform->addChild(testCube);
}

}  // End namespace Core
}  // End namespace Updraft

