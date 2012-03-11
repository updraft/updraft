#include "openedfile.h"

#include <QDebug>

#include <osg/Geode>
#include <osg/LineWidth>

#include "igcinfo.h"

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
  tab = NULL;  // tab will be deleted after we drop out of this function

  for (int i = 0; i < colorsCombo->model()->rowCount(); ++i) {
    delete colorsCombo->itemData(i).value<IgcInfo*>();
  }

  delete this;
}

void OpenedFile::createTab() {
  colorsCombo = new QComboBox();

  #define ADD_COLORING(name, pointer) \
    colorsCombo->addItem(name, \
      QVariant::fromValue(static_cast<IgcInfo*>(pointer)))

  ADD_COLORING(tr("Vertical Speed"), new VerticalSpeedIgcInfo());
  ADD_COLORING(tr("Ground Speed"), new GroundSpeedIgcInfo());
  ADD_COLORING(tr("Altitude"), new AltitudeIgcInfo());
  ADD_COLORING(tr("Red"), new ConstantIgcInfo(Qt::red));
  ADD_COLORING(tr("Green"), new ConstantIgcInfo(Qt::green));
  ADD_COLORING(tr("Blue"), new ConstantIgcInfo(Qt::blue));
  ADD_COLORING(tr("Gray"), new ConstantIgcInfo(Qt::gray));
  ADD_COLORING(tr("Yellow"), new ConstantIgcInfo(Qt::yellow));

  tab = viewer->core->createTab(colorsCombo, fileInfo.fileName());

  tab->connectSignalClosed(this, SLOT(close()));
  connect(colorsCombo, SIGNAL(currentIndexChanged(int)),
    this, SLOT(updateColors(int)));

  updateColors(colorsCombo->currentIndex());
}

void OpenedFile::updateColors(int row) {
  setColors(colorsCombo->itemData(row).value<IgcInfo*>());
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

    /// \todo fill terrain height
    fixList.append(TrackFix(ev->timestamp, igcFix->gpsLoc, x, y, z, 0));
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

