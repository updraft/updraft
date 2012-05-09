#include "openedfile.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <osg/Geode>
#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osg/Vec3>

#include "pluginbase.h"
#include "igc/igc.h"

#include "plotwidget.h"

namespace Updraft {
namespace IgcViewer {

OpenedFile::~OpenedFile() {
  if (tab) {
    TabInterface *tmp = tab;
    tab = NULL;
    delete tmp;
  }

  foreach(IgcInfo* info, igcInfo) {
    delete info;
  }

  foreach(Coloring* coloring, colorings) {
    delete coloring;
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

  const Util::Ellipsoid* ellipsoid = g_core->getEllipsoid();

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

    ellipsoid->getOsgEllipsoidModel()->convertLatLongHeightToXYZ(
      igcFix->gpsLoc.lat_radians(), igcFix->gpsLoc.lon_radians(),
      igcFix->gpsLoc.alt,
      x, y, z);

    /// \todo fill terrain height
    fixList.append(TrackFix(ev->timestamp, igcFix->gpsLoc, x, y, z, 0));
  }

  return true;
}

bool OpenedFile::init(IgcViewer* viewer,
  const QString& filename, QColor color) {
  this->viewer = viewer;
  fileInfo = QFileInfo(filename);
  automaticColor = color;

  if (!loadIgc(filename)) {
    return false;
  }

  colorsCombo = new QComboBox();
  textBox = new IGCTextWidget();
  textBox->setReadOnly(true);
  textBox->setFixedSize(100, 300);

  gradient = Util::Gradient(Qt::blue, Qt::red, true);

  #define ADD_IGCINFO(variable, pointer) \
    do { \
      igcInfo.append(pointer); \
      igcInfo[igcInfo.count() - 1]->init(&fixList); \
      variable = igcInfo[igcInfo.count() - 1]; \
    } while (0)

  ADD_IGCINFO(altitudeInfo, new AltitudeIgcInfo());
  ADD_IGCINFO(verticalSpeedInfo, new VerticalSpeedIgcInfo());
  ADD_IGCINFO(groundSpeedInfo, new GroundSpeedIgcInfo());
  ADD_IGCINFO(timeInfo, new TimeIgcInfo());

  #define ADD_COLORING(name, pointer) \
    do { \
      colorings.append(pointer); \
      colorsCombo->addItem(name); \
    } while (0)

  ADD_COLORING(tr("Automatic"),
    new ConstantColoring(color));
  ADD_COLORING(tr("Vertical Speed"),
    new SymmetricColoring(verticalSpeedInfo, &gradient));
  ADD_COLORING(tr("Ground Speed"),
    new DefaultColoring(groundSpeedInfo, &gradient));
  ADD_COLORING(tr("Altitude"),
    new DefaultColoring(altitudeInfo, &gradient));
  ADD_COLORING(tr("Time"),
    new LocalColoring(timeInfo, &gradient));


  QWidget* tabWidget = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout();
  QVBoxLayout* verticalLayout = new QVBoxLayout();

  layout->setContentsMargins(0, 0, 0, 0);

  plotWidget = new PlotWidget(
    altitudeInfo, verticalSpeedInfo, groundSpeedInfo);

  connect(plotWidget, SIGNAL(updateCurrentInfo(const QString&)),
    textBox, SLOT(setMouseOverText(const QString&)));
  connect(plotWidget, SIGNAL(updatePickedInfo(const QString&)),
    textBox, SLOT(setPickedText(const QString&)));
  connect(plotWidget, SIGNAL(timeWasPicked(QTime)),
    this, SLOT(timePicked(QTime)));
  connect(plotWidget, SIGNAL(displayMarker(bool)),
    this, SLOT(displayMarker(bool)));

  tabWidget->setLayout(layout);
  verticalLayout->addWidget(colorsCombo, 0, Qt::AlignTop);
  verticalLayout->addWidget(textBox, 1, Qt::AlignTop);
  layout->addLayout(verticalLayout, 0);
  layout->addWidget(plotWidget, 1.0);

  tab = g_core->createTab(tabWidget, fileInfo.fileName());

  tab->connectSignalCloseRequested(this, SLOT(close()));
  connect(colorsCombo, SIGNAL(currentIndexChanged(int)),
    viewer, SLOT(coloringChanged(int)));

  // Sets automatic tab closing on request (without prompt).
  tab->connectCloseRequestToClose();

  createTrack();

  coloringChanged();

  return true;
}

void OpenedFile::redraw() {
  setColors(colorings[viewer->currentColoring]);
  g_core->requestRedraw();
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

void OpenedFile::coloringChanged() {
  colorsCombo->setCurrentIndex(viewer->currentColoring);
  redraw();
}

void OpenedFile::createTrack() {
  sceneRoot = new osg::Group();
  trackGeode = new osg::Geode();

  geom = new osg::Geometry();
  trackGeode->addDrawable(geom);

  sceneRoot->addChild(trackGeode);

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

  osg::StateSet* stateSet = trackGeode->getOrCreateStateSet();
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
  stateSet->setAttributeAndModes(new osg::LineWidth(3));
  // stateSet->setAttributeAndModes(
  //   new osg::LineWidth(viewer->lineWidthSetting->get().toFloat()));

    // create the marker
  trackPositionMarker = createMarker();
  trackPositionMarker->setNodeMask(0x0);  // make it invisible first
  // currentMarkerTransform = new osg::PositionAttitudeTransform();
  currentMarkerTransform = new osg::AutoTransform();
  // currentMarkerTransform->setAutoScaleToScreen(true);
  currentMarkerTransform->setMinimumScale(10);
  currentMarkerTransform->setMaximumScale(500);
  currentMarkerTransform->setScale(500);
  currentMarkerTransform->addChild(trackPositionMarker);
  // currentMarkerTransform->setScale(osg::Vec3d(100., 100., 100.));
  sceneRoot->addChild(currentMarkerTransform);

    // push the scene
  track = viewer->mapLayerGroup->insertMapLayer(sceneRoot, fileInfo.fileName());
  track->connectCheckedToVisibility();
}

void OpenedFile::setColors(Coloring *coloring) {
  currentColoring = coloring;

  osg::Vec4Array* colors = new osg::Vec4Array();

  for (int i = 0; i < fixList.count(); ++i) {
    QColor color = coloring->color(i);
    colors->push_back(osg::Vec4(
      color.redF(), color.greenF(), color.blueF(), color.alphaF()));
  }

  geom->setColorArray(colors);
}

void OpenedFile::updateScales(const OpenedFile *other) {
  for (int i = 0; i < igcInfo.count(); ++i) {
    igcInfo[i]->addGlobalScale(other->igcInfo[i]);
  }
}

void OpenedFile::selectTab() {
  tab->select();
}

void OpenedFile::resetScales() {
  for (int i = 0; i < igcInfo.count(); ++i) {
    igcInfo[i]->resetGlobalScale();
  }
}

QString OpenedFile::fileName() {
  return fileInfo.absoluteFilePath();
}

osg::Node* OpenedFile::getNode() {
  return trackGeode;
}

void OpenedFile::trackClicked(const EventInfo* eventInfo) {
    // find nearest fix:
  if (fixList.empty()) return;
    // index of nearest trackFix
  QList<TrackFix>::iterator nearest = fixList.begin();
  float dx = nearest->x - eventInfo->intersection.x();
  float dy = nearest->y - eventInfo->intersection.y();
  float dz = nearest->z - eventInfo->intersection.z();
  float distance = dx*dx + dy*dy + dz*dz;
  float minDistance = distance;

  for (QList<TrackFix>::iterator it = fixList.begin();
    it != fixList.end(); it++) {
    dx = it->x - eventInfo->intersection.x();
    dy = it->y - eventInfo->intersection.y();
    dz = it->z - eventInfo->intersection.z();
    distance = dx*dx + dy*dy + dz*dz;
    if (distance < minDistance) {
      minDistance = distance;
      nearest = it;
    }
  }

  qDebug() << minDistance;

  QTime& time = nearest->timestamp;
  qreal timeSecs = time.hour()*3600 + time.minute()*60 + time.second();
  plotWidget->setPickedTime(timeSecs);

  currentMarkerTransform->setPosition(
    osg::Vec3d(nearest->x, nearest->y, nearest->z));
  displayMarker(true);
}

void OpenedFile::timePicked(QTime time) {
    // find fix with nearest time:
  if (fixList.empty()) return;
    // index of nearest trackFix

  QList<TrackFix>::iterator nearest = fixList.begin();
  float distance = qAbs(time.secsTo(nearest->timestamp));
  float minDistance = distance;

  for (QList<TrackFix>::iterator it = fixList.begin();
    it != fixList.end(); it++) {
    distance = qAbs(time.secsTo(it->timestamp));
    if (distance < minDistance) {
      minDistance = distance;
      nearest = it;
    }
  }

    // do the transformation of the marker to the position
    // of the nearest trackFix.
  currentMarkerTransform->setPosition(
    osg::Vec3(nearest->x, nearest->y, nearest->z));
}

osg::Node* OpenedFile::createMarker() {
  osg::Sphere* unitSphere = new osg::Sphere(osg::Vec3(0, 0, 0), 1.0);
  osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);
  osg::Geode* unitSphereGeode = new osg::Geode();
  unitSphereGeode->addDrawable(unitSphereDrawable);
  return unitSphereGeode;
}

void OpenedFile::displayMarker(bool value) {
  if (value) {
    trackPositionMarker->setNodeMask(0xffffffff);
  } else {
    trackPositionMarker->setNodeMask(0x0);
  }
}

}  // End namespace IgcViewer
}  // End namespace Updraft

