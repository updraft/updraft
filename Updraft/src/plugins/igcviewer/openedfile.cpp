#include "openedfile.h"

#include <QComboBox>
#include <QDebug>

#include <osg/Geode>
#include <osg/LineWidth>

#include "igc/igc.h"

#include "igcinfo.h"

namespace Updraft {
namespace IgcViewer {

OpenedFile::~OpenedFile() {
  if (tab) {
    TabInterface *tmp = tab;
    tab = NULL;
    delete tmp;
  }

  foreach(IgcInfo* info, igcInfoList) {
    delete info;
  }

  viewer->mapLayerGroup->removeMapLayer(track);

  viewer->fileClose(this);
}

bool OpenedFile::loadIgc(const QString& filename) {
  Igc::IgcFile igc;

  if (!igc.load(filename)) {
    qDebug() << "Loading IGC file failed.";
    return false;
  }

  const osg::EllipsoidModel* ellipsoid = viewer->core->getEllipsoidModel();

  fixList.clear();

  foreach(Igc::Event const* ev, igc.events()) {
    if (ev->type != Igc::Event::FIX) {
      continue;
    }
    Igc::Fix const* igcFix = static_cast<Igc::Fix const*>(ev);

    if (!igcFix->valid) {
      continue;
    }

    qreal x, y, z;

    ellipsoid->convertLatLongHeightToXYZ(
      igcFix->gpsLoc.lat_radians(), igcFix->gpsLoc.lon_radians(),
      igcFix->gpsLoc.alt,
      x, y, z);

    /// \todo fill terrain height
    fixList.append(TrackFix(ev->timestamp, igcFix->gpsLoc, x, y, z, 0));
  }

  return true;
}

bool OpenedFile::init(IgcViewer* viewer, const QString& filename) {
  this->viewer = viewer;

  fileInfo = QFileInfo(filename);

  if (!loadIgc(filename)) {
    return false;
  }

  QComboBox *colorsCombo = new QComboBox();

  #define ADD_IGCINFO(name, pointer) \
    do { \
      igcInfoList.append(pointer); \
      igcInfoList[igcInfoList.count() - 1]->init(&fixList); \
      colorsCombo->addItem(name); \
    } while (0)

  ADD_IGCINFO(tr("Vertical Speed"), new VerticalSpeedIgcInfo());
  verticalSpeedInfo = igcInfoList[igcInfoList.count() - 1];
  ADD_IGCINFO(tr("Ground Speed"), new GroundSpeedIgcInfo());
  ADD_IGCINFO(tr("Altitude"), new AltitudeIgcInfo());
  altitudeInfo = igcInfoList[igcInfoList.count() - 1];
  ADD_IGCINFO(tr("Red"), new ConstantIgcInfo(Qt::red));
  ADD_IGCINFO(tr("Green"), new ConstantIgcInfo(Qt::green));
  ADD_IGCINFO(tr("Blue"), new ConstantIgcInfo(Qt::blue));
  ADD_IGCINFO(tr("Gray"), new ConstantIgcInfo(Qt::gray));
  ADD_IGCINFO(tr("Yellow"), new ConstantIgcInfo(Qt::yellow));

  tab = viewer->core->createTab(colorsCombo, fileInfo.fileName());

  tab->connectSignalClosed(this, SLOT(close()));
  connect(colorsCombo, SIGNAL(currentIndexChanged(int)),
    this, SLOT(updateColors(int)));

  createTrack();

  updateColors(colorsCombo->currentIndex());

  return true;
}

void OpenedFile::redraw() {
  setColors(currentColoring);
}

void OpenedFile::close() {
  if (!tab) {
    // If we were destroying this file starting from the regular consturctor,
    // then the tab is already set to null and we don't have to do anything.
    return;
  }

  tab = NULL;  // tab will be deleted after we drop out of this function
  delete this;
}

void OpenedFile::updateColors(int row) {
  setColors(igcInfoList[row]);
}

void OpenedFile::createTrack() {
  osg::Geode* geode = new osg::Geode();

  geom = new osg::Geometry();
  geode->addDrawable(geom);

  osg::DrawArrays* drawArrayLines =
    new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
  geom->addPrimitiveSet(drawArrayLines);

  osg::Vec3Array* vertices = new osg::Vec3Array();
  geom->setVertexArray(vertices);

  geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

  foreach(TrackFix fix, fixList) {
    vertices->push_back(osg::Vec3(fix.x, fix.y, fix.z));
  }

  drawArrayLines->setFirst(0);
  drawArrayLines->setCount(vertices->size());

  osg::StateSet* stateSet = geode->getOrCreateStateSet();
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
  stateSet->setAttributeAndModes(new osg::LineWidth(3));
  // stateSet->setAttributeAndModes(
  //   new osg::LineWidth(viewer->lineWidthSetting->get().toFloat()));

  track = viewer->mapLayerGroup->insertMapLayer(geode, fileInfo.fileName());
  track->connectDisplayedToVisibility();
}

void OpenedFile::setColors(IgcInfo *coloring) {
  currentColoring = coloring;

  osg::Vec4Array* colors = new osg::Vec4Array();

  coloring->init(&fixList);

  for (int i = 0; i < fixList.count(); ++i) {
    QColor color = coloring->color(i);
    colors->push_back(osg::Vec4(
      color.redF(), color.greenF(), color.blueF(), color.alphaF()));
  }

  geom->setColorArray(colors);
}

}  // End namespace IgcViewer
}  // End namespace Updraft

