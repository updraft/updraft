#include <math.h>

#include <QtXml/QDomDocument>
#include <osg/CoordinateSystemNode>

#include "taskdata.h"
#include "taskpoint.h"

namespace Updraft {

TaskData::~TaskData() {
  foreach(TaskPoint* point, taskPoints) {
    delete point;
  }

  taskPoints.clear();
}

QString TaskData::toXml() const {
  QDomDocument doc;

  doc.appendChild(doc.createProcessingInstruction(
    "xml", "version=\"1.0\" encoding=\"UTF-8\""));

  // Creates TaskData root element.
  QDomElement root = doc.createElement("TaskData");
  doc.appendChild(root);

  // Creates xml elements for each task points.
  foreach(TaskPoint* taskPoint, taskPoints) {
    QDomElement taskPointElem = doc.createElement("TaskPoint");
    root.appendChild(taskPointElem);

    taskPoint->toDom(&doc, &taskPointElem);
  }

  return doc.toString();
}

TaskPoint* TaskData::getTaskPoint(int position) {
  if (position >= taskPoints.size()) {
    return NULL;
  }

  return taskPoints[position];
}

const TaskPoint* TaskData::getTaskPoint(int position) const {
  if (position >= taskPoints.size()) {
    return NULL;
  }

  return taskPoints[position];
}

bool TaskData::fromXml(const QString &serialized) {
  QDomDocument doc;
  if (!doc.setContent(serialized)) {
    return false;
  }

  QDomElement taskDataElem = doc.firstChildElement("TaskData");
  if (!taskDataElem.isElement()) {
    return false;
  }

  QDomElement taskPointElem = taskDataElem.firstChildElement("TaskPoint");

  while (!taskPointElem.isNull()) {
    // Creates new TaskPoint and appends it to vector.
    TaskPoint *taskPoint = new TaskPoint(taskPointElem);
    taskPoints.push_back(taskPoint);

    taskPointElem = taskPointElem.nextSiblingElement("TaskPoint");
  }

  return true;
}

void TaskData::insertTaskPoint(TaskPoint *taskPoint, int position) {
  if (taskPoint == NULL) {
    return;
  }

  if (position < 0 || position >= taskPoints.size()) {
    position = taskPoints.size();
  }

  taskPoints.insert(position, taskPoint);
}

void TaskData::moveTaskPoint(int from, int to) {
  if (from < 0 || from >= taskPoints.size()) {
    return;
  }

  if (to < 0 || to >= taskPoints.size()) {
    to = taskPoints.size()-1;
  }

  TaskPoint *taskPoint = taskPoints[from];

  taskPoints.erase(taskPoints.begin() + from);
  taskPoints.insert(to, taskPoint);
}

void TaskData::deleteTaskPoint(int position) {
  if (position < 0 || position >= taskPoints.size()) {
    return;
  }

  taskPoints.erase(taskPoints.begin() + position);
}

TaskData::TaskData(const osg::EllipsoidModel* ellipsoid)
  : ellipsoid(ellipsoid) { }

TaskData::TaskData(const TaskData& taskData)
  : taskPoints(taskData.taskPoints), ellipsoid(taskData.ellipsoid) { }

qreal TaskData::distanceFrom(int i) const {
  // TODO(Kuba): What is a correct way to calculate distance between TPs?

  Util::Location loc1 = taskPoints[i]->getLocation();
  double x1, y1, z1;
  ellipsoid->convertLatLongHeightToXYZ(
    loc1.lat_radians(), loc1.lon_radians(), loc1.alt,
    x1, y1, z1);

  Util::Location loc2 = taskPoints[i + 1]->getLocation();
  double x2, y2, z2;
  ellipsoid->convertLatLongHeightToXYZ(
    loc2.lat_radians(), loc2.lon_radians(), loc2.alt,
    x2, y2, z2);

  return sqrt(
    (x2 - x1) * (x2 - x1) +
    (y2 - y1) * (y2 - y1) +
    (z2 - z1) * (z2 - z1));
}

qreal TaskData::totalLength() const {
  qreal sum = 0;
  for (int i = 0; i < taskPoints.size() - 1; ++i) {
    sum += distanceFrom(i);
  }
  return sum;
}
}  // End namespace Updraft
