#include "openedfile.h"

#include <QDebug>

#include <osg/Geode>
#include <osg/LineWidth>

#include "colorings.h"

namespace Updraft {
namespace IgcViewer {

OpenedFile::~OpenedFile() {
  if (tab) {
    delete tab;
  }
  viewer->mapLayerGroup->removeMapLayer(track);
  viewer->opened.removeAll(this);
}

bool OpenedFile::init(IgcViewer* viewer, const QString& filename) {
  this->viewer = viewer;

  fileInfo = QFileInfo(filename);

  if (!igc.load(filename)) {
    qDebug() << "Loading IGC file failed.";
    return false;
  }

  createTrack();
  createTab();

  return true;
}

void OpenedFile::close() {
  tab = NULL;  // tab is already deleted
  delete this;
}

void OpenedFile::createTab() {
  colorsCombo = new QComboBox();

  /// \bug The following allocations cause a memmory leak!

  #define ADD_COLORING(name, pointer) \
    colorsCombo->addItem(name, \
      QVariant::fromValue(static_cast<Coloring*>(pointer)))

  ADD_COLORING(tr("Red"), new ConstantColoring(Qt::red));
  ADD_COLORING(tr("Green"), new ConstantColoring(Qt::green));
  ADD_COLORING(tr("Blue"), new ConstantColoring(Qt::blue));
  ADD_COLORING(tr("Black"), new ConstantColoring(Qt::black));
  ADD_COLORING(tr("Yellow"), new ConstantColoring(Qt::yellow));
  ADD_COLORING(tr("Altitude"), new AltitudeColoring());
  ADD_COLORING(tr("Ground Speed"), new GroundSpeedColoring());

  tab = viewer->core->createTab(colorsCombo, fileInfo.fileName());

  tab->connectSignalClosed(this, SLOT(close()));
  connect(colorsCombo, SIGNAL(currentIndexChanged(int)),
    this, SLOT(updateColors(int)));

  updateColors(colorsCombo->currentIndex());
}

void OpenedFile::updateColors(int row) {
  setColors(colorsCombo->itemData(row).value<Coloring*>());
}

void OpenedFile::createTrack() {
  const osg::EllipsoidModel* ellipsoid = viewer->core->getEllipsoidModel();

  osg::Geode* geode = new osg::Geode();

  geom = new osg::Geometry();
  geode->addDrawable(geom);

  osg::DrawArrays* drawArrayLines =
    new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
  geom->addPrimitiveSet(drawArrayLines);

  osg::Vec3Array* vertices = new osg::Vec3Array();
  geom->setVertexArray(vertices);

  geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

  foreach(Igc::Event const* ev, igc.events()) {
    if (ev->type != Igc::Event::FIX) {
      continue;
    }
    Igc::Fix const* igcFix = static_cast<Igc::Fix const*>(ev);

    if (!igcFix->valid) {
      continue;
    }

    double x, y, z;

    ellipsoid->convertLatLongHeightToXYZ(
      igcFix->gpsLoc.lat_radians(), igcFix->gpsLoc.lon_radians(),
      igcFix->gpsLoc.alt,
      x, y, z);

    vertices->push_back(osg::Vec3(x, y, z));
    fixList.append(TrackFix(ev->timestamp, igcFix->gpsLoc, x, y, z));
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

void OpenedFile::setColors(Coloring *coloring) {
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

