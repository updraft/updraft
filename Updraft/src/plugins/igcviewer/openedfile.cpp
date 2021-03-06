#include "openedfile.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>

#include <osg/Depth>
#include <osg/Geode>
#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osg/Vec3>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/BlendFunc>

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

  delete igc;

  foreach(FixInfo* info, fixInfo) {
    delete info;
  }
  fixInfo.clear();

  foreach(Coloring* coloring, colorings) {
    delete coloring;
  }
  colorings.clear();

  viewer->mapLayerGroup->removeMapLayer(track);

  viewer->fileClose(this);
}

bool OpenedFile::loadIgc(const QString& filename) {
  igc = new Igc::IgcFile();
  if (!igc->load(filename)) {
    qDebug() << "Loading IGC file failed.";
    return false;
  }

  const osg::EllipsoidModel* ellipsoid = g_core->getCurrentMapEllipsoid();

  fixList.clear();

  foreach(Igc::Event const* ev, igc->events()) {
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

bool OpenedFile::init(IgcViewer* viewer,
  const QString& filename, QColor color) {
  this->viewer = viewer;
  fileInfo = QFileInfo(filename);
  automaticColor = color;

  if (!loadIgc(filename)) {
    return false;
  }

  colorsCombo = new QComboBox();

  gradient = Util::Gradient(Qt::blue, Qt::red, true);

  #define ADD_IGCINFO(variable, pointer) \
    do { \
      fixInfo.append(pointer); \
      fixInfo[fixInfo.count() - 1]->init(&fixList); \
      variable = fixInfo[fixInfo.count() - 1]; \
    } while (0)

  ADD_IGCINFO(altitudeInfo, new AltitudeFixInfo());
  ADD_IGCINFO(verticalSpeedInfo, new VerticalSpeedFixInfo());
  ADD_IGCINFO(groundSpeedInfo, new GroundSpeedFixInfo());
  ADD_IGCINFO(timeInfo, new TimeFixInfo());

  SegmentInfo* segmentInfo = new SegmentInfo();
  segmentInfo->init(&fixList);

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
  QGridLayout* layout = new QGridLayout();

  layout->setContentsMargins(0, 0, 0, 0);

  plotWidget = new PlotWidget(
    segmentInfo, altitudeInfo, verticalSpeedInfo, groundSpeedInfo);

  textBox = new IgcTextWidget(plotWidget->getSegmentsStatTexts(),
    plotWidget->getPointsStatTexts());
  textBox->setReadOnly(true);

  QLabel* header = new QLabel();
  setHeaderText(header);

  tabWidget->setLayout(layout);
  layout->addWidget(colorsCombo, 0, 0);
  layout->addWidget(textBox, 1, 0);
  layout->addWidget(header, 0, 1);
  layout->addWidget(plotWidget, 1, 1);

  tab = g_core->createTab(tabWidget, fileInfo.fileName());

  tab->connectSignalCloseRequested(this, SLOT(close()));
  connect(colorsCombo, SIGNAL(currentIndexChanged(int)),
    viewer, SLOT(coloringChanged(int)));

  // Sets automatic tab closing on request (without prompt).
  tab->connectCloseRequestToClose();

  createGroup();

  coloringChanged();

  connect(plotWidget, SIGNAL(updateCurrentInfo(const QString&)),
    textBox, SLOT(setMouseOverText(const QString&)));
  connect(plotWidget, SIGNAL(updateText()),
    textBox, SLOT(updateText()));
  connect(plotWidget, SIGNAL(fixWasPicked(int)),
    this, SLOT(fixPicked(int)));
  connect(plotWidget, SIGNAL(fixIsPointedAt(int)),
    this, SLOT(fixIsPointedAt(int)));
  connect(plotWidget, SIGNAL(clearMarkers()),
    this, SLOT(clearMarkers()));

  return true;
}

void OpenedFile::redraw() {
  setColors(colorings[viewer->currentColoring]);
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
  setColors(colorings[viewer->currentColoring]);
}

void OpenedFile::createGroup() {
  sceneRoot = new osg::Group();

  sceneRoot->addChild(createTrack());
  sceneRoot->addChild(createSkirt());

  // create marker geometry
  trackPositionMarker = createMarker(25.);
  trackPositionMarker->setNodeMask(0xffffffff);

  // create marker that points to location of the mouse point
  currentMarker = new osg::AutoTransform();
  currentMarker->setAutoRotateMode(
    osg::AutoTransform::ROTATE_TO_SCREEN);
  currentMarker->setAutoScaleToScreen(true);
  currentMarker->setMinimumScale(0.1);
  currentMarker->setMaximumScale(100);
  currentMarker->addChild(createMarker(25.));
  currentMarker->setPosition(osg::Vec3d(0, 0, 0));
  currentMarker->setNodeMask(0x0);

  sceneRoot->addChild(currentMarker);

  // push the scene
  track = viewer->mapLayerGroup->createMapLayer(sceneRoot, fileInfo.fileName());
  track->connectCheckedToVisibility();
  track->connectSignalContextMenuRequested(this,
    SLOT(contextMenuRequested(QPoint, MapLayerInterface*)));
  track->zoom();
}

osg::Node* OpenedFile::createTrack() {
  trackGeode = new osg::Geode();
  geom = new osg::Geometry();

  trackGeode->addDrawable(geom);

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

  return trackGeode;
}

osg::Node* OpenedFile::createSkirt() {
  osg::Geode *geode = new osg::Geode();
  osg::Geometry *skirtGeom = new osg::Geometry();

  geode->addDrawable(skirtGeom);

  osg::DrawArrays* drawArray =
    new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP);
  skirtGeom->addPrimitiveSet(drawArray);

  osg::Vec3Array* vertices = new osg::Vec3Array();
  skirtGeom->setVertexArray(vertices);

  skirtGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

  const osg::EllipsoidModel* ellipsoid =
    g_core->getCurrentMapEllipsoid();
  foreach(TrackFix fix, fixList) {
    double x, y, z;
    ellipsoid->convertLatLongHeightToXYZ(
      fix.location.lat_radians(), fix.location.lon_radians(),
      0,
      x, y, z);
    vertices->push_back(osg::Vec3(fix.x, fix.y, fix.z));
    vertices->push_back(osg::Vec3(x, y, z));
  }

  drawArray->setFirst(0);
  drawArray->setCount(vertices->size());

  osg::Vec4Array* color = new osg::Vec4Array();
  color->push_back(osg::Vec4(0.5, 0.5, 0.5, 0.5));
  skirtGeom->setColorArray(color);

  osg::StateSet* stateSet = geode->getOrCreateStateSet();
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
  stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
  stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

  osg::Depth* depth = new osg::Depth;
  depth->setWriteMask(false);
  stateSet->setAttributeAndModes(depth, osg::StateAttribute::ON);

  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

  return geode;
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
  for (int i = 0; i < fixInfo.count(); ++i) {
    fixInfo[i]->addGlobalScale(other->fixInfo[i]);
  }
}

void OpenedFile::selectTab() {
  tab->select();
}

void OpenedFile::resetScales() {
  for (int i = 0; i < fixInfo.count(); ++i) {
    fixInfo[i]->resetGlobalScale();
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
  int nearest = 0;
  float dx = fixList[nearest].x - eventInfo->intersection.x();
  float dy = fixList[nearest].y - eventInfo->intersection.y();
  float dz = fixList[nearest].z - eventInfo->intersection.z();
  float distance = dx*dx + dy*dy + dz*dz;
  float minDistance = distance;

  for (int i = 0; i < fixList.count(); i++) {
    dx = fixList[i].x - eventInfo->intersection.x();
    dy = fixList[i].y - eventInfo->intersection.y();
    dz = fixList[i].z - eventInfo->intersection.z();
    distance = dx*dx + dy*dy + dz*dz;
    if (distance < minDistance) {
      minDistance = distance;
      nearest = i;
    }
  }

  plotWidget->addPickedFix(nearest);

  osg::AutoTransform* currentMarkerTransform = new osg::AutoTransform();
  currentMarkerTransform->setAutoRotateMode(
    osg::AutoTransform::ROTATE_TO_SCREEN);
  currentMarkerTransform->setAutoScaleToScreen(true);
  currentMarkerTransform->setMinimumScale(0.1);
  currentMarkerTransform->setMaximumScale(100);
  currentMarkerTransform->addChild(trackPositionMarker);
  sceneRoot->addChild(currentMarkerTransform);
  currentMarkerTransform->setPosition(
    osg::Vec3(fixList[nearest].x, fixList[nearest].y, fixList[nearest].z));
  pickedMarkers.append(currentMarkerTransform);
}

void OpenedFile::fixPicked(int index) {
    // find fix with nearest time:
  if (fixList.empty()) return;

  // create a new transform node:
  osg::AutoTransform* currentMarkerTransform = new osg::AutoTransform();
  currentMarkerTransform->setAutoRotateMode(
    osg::AutoTransform::ROTATE_TO_SCREEN);
  currentMarkerTransform->setAutoScaleToScreen(true);
  currentMarkerTransform->setMinimumScale(0.1);
  currentMarkerTransform->setMaximumScale(100);
  currentMarkerTransform->addChild(trackPositionMarker);
  sceneRoot->addChild(currentMarkerTransform);
  currentMarkerTransform->setPosition(
    osg::Vec3(fixList[index].x, fixList[index].y, fixList[index].z));
  pickedMarkers.append(currentMarkerTransform);
}

void OpenedFile::fixIsPointedAt(int index) {
    // find fix with nearest time:
  if ((index < 0) || (index >= fixList.count())) {
    // no fix was picked, hide the marker
    currentMarker->setNodeMask(0x0);
  } else {
    currentMarker->setNodeMask(0xffffffff);
      // set the position of the current marker node:
    currentMarker->setPosition(
      osg::Vec3(fixList[index].x, fixList[index].y, fixList[index].z));
  }
}

osg::Geode* OpenedFile::createMarker(qreal scale) {
  osg::Geode* geode = new osg::Geode();
  osg::Geometry* geometry = new osg::Geometry();

  osg::Vec3Array* vertices = new osg::Vec3Array();
  osg::Vec3 vertex(scale, 0, 0);

  int nVert = 8;
  qreal step = 2*M_PI / (qreal)nVert;
  qreal t = 0;
  for (int i = 0; i < nVert; i++) {
    t += step;
    vertices->push_back(vertex);
    vertex = osg::Vec3(scale*cos(t), scale*sin(t), 0);
    vertices->push_back(vertex);
  }

  vertices->push_back(osg::Vec3(0, scale*1.2, 0.0));
  vertices->push_back(osg::Vec3(0, -scale*1.2, 0.0));
  vertices->push_back(osg::Vec3(scale*1.2, 0, 0.0));
  vertices->push_back(osg::Vec3(-scale*1.2, 0, 0.0));

  geometry->setVertexArray(vertices);

  osg::Vec4Array* color = new osg::Vec4Array();
  for (unsigned int i = 0; i < vertices->size()-4; i++) {
    color->push_back(osg::Vec4(1.0, 1.0, 0.0, 1.0));
  }
  for (int i = 0; i < 4; i++) {
    color->push_back(osg::Vec4(0.0, 0.0, 0.0, 1.0));
  }

  geometry->setColorArray(color);
  geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

  geometry->addPrimitiveSet(new osg::DrawArrays(
    osg::PrimitiveSet::LINES, 0, vertices->size()));

  osg::StateSet* stateSet = geode->getOrCreateStateSet();

  // Turn off lighting for the billboard.
  stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
  stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

  // Turn on blending.
  stateSet->setAttributeAndModes(new osg::BlendFunc());
  stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

  // Set line width.
  stateSet->setAttributeAndModes(new osg::LineWidth(2));

  geode->addDrawable(geometry);

  return geode;
}

void OpenedFile::setHeaderText(QLabel* label) {
  QString text;
  QDate date = igc->date();
  if (date.isValid()) {
    text += date.toString(Qt::DefaultLocaleShortDate) + "\t";
  }
  QString pilot = igc->pilot();
  if (!pilot.isEmpty()) {
    text += tr("Pilot: ") + pilot + "\t";
  }
  QString glider = igc->gliderType();
  if (!glider.isEmpty()) {
    text += tr("Glider: ") + glider;
  }
  label->setText(text);
}

void OpenedFile::clearMarkers() {
  for (int i = 0; i < pickedMarkers.size(); i++) {
    sceneRoot->removeChild(pickedMarkers[i]);
  }
  pickedMarkers.clear();
}

void OpenedFile::contextMenuRequested(QPoint pos, MapLayerInterface* sender) {
  QMenu menu;
  menu.addAction(sender->getZoomAction());
  menu.exec(pos);
}

}  // End namespace IgcViewer
}  // End namespace Updraft

