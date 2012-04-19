#include <math.h>

#include <QDebug>
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
  : ellipsoid(taskData.ellipsoid) {
  // Deep copy of taskPoints container
  foreach(TaskPoint *point, taskData.taskPoints) {
    taskPoints.push_back(new TaskPoint(*point));
  }
}

bool TaskData::isFaiTriangle() const {
  return faiTriangleDistance() >= 0;
}

qreal TaskData::distance(int i, int j) const {
  // TODO(Kuba): What is a correct way to calculate distance between TPs?

  Util::Location loc1 = taskPoints[i]->getLocation();
  double x1, y1, z1;
  ellipsoid->convertLatLongHeightToXYZ(
    loc1.lat_radians(), loc1.lon_radians(), loc1.alt,
    x1, y1, z1);

  Util::Location loc2 = taskPoints[j]->getLocation();
  double x2, y2, z2;
  ellipsoid->convertLatLongHeightToXYZ(
    loc2.lat_radians(), loc2.lon_radians(), loc2.alt,
    x2, y2, z2);

  return sqrt(
    (x2 - x1) * (x2 - x1) +
    (y2 - y1) * (y2 - y1) +
    (z2 - z1) * (z2 - z1));
}

qreal TaskData::totalDistance() const {
  qreal sum = 0;
  for (int i = 0; i < size() - 1; ++i) {
    sum += distance(i, i + 1);
  }
  return sum;
}

qreal TaskData::distanceReduction(int i) const {
    AssignedArea aa = taskPoints[i]->getAssignedArea();

    if (aa.isCylinder()) {
      return 1000 * aa.maxRadius;
    }
    return 0;
}

qreal TaskData::officialDistance() const {
  if (size() < 2) {
    return 0;
  }

  qreal dist = faiTriangleDistance();

  if (dist >= 0) {
    return dist;
  }

  dist = totalDistance();
  dist -= distanceReduction(0);
  for (int i = 1; i < size() - 1; ++i) {
    dist -= 2 * distanceReduction(i);
  }
  dist -= distanceReduction(size() - 1);

  return dist;
}

float TaskData::faiTriangleDistance() const {
  if (!isClosed()) {
    // qDebug() << "Not closed";
    return -1;
  }

  qreal a, b, c;  // legs of the triangle

  if (size() == 4) {
    a = distance(0, 1);
    b = distance(1, 2);
    c = distance(2, 3);
  }

  if (size() == 5) {
    a = distance(1, 2);
    b = distance(2, 3);
    c = distance(3, 1);
  }

  qreal officialDist = a + b + c;

  if (size() == 4) {
    officialDist -= distanceReduction(0);
    officialDist -= 2 * distanceReduction(1);
    officialDist -= 2 * distanceReduction(2);
    officialDist -= distanceReduction(3);
  }

  if (size() == 5) {
    officialDist -= 2 * distanceReduction(1);
    officialDist -= 2 * distanceReduction(2);
    officialDist -= 2 * distanceReduction(3);

    if (officialDist < 300000) {
      // qDebug() << "too short";
      return -1;
    }
  }

  if (officialDist > 750000) {
    qreal minLeg = 0.25 * officialDist;
    qreal maxLeg = 0.45 * officialDist;

    if (
      a < minLeg || a > maxLeg ||
      b < minLeg || b > maxLeg ||
      c < minLeg || c > maxLeg) {
      // qDebug() << "large, invalid dimensions";
      return -1;
    }
  } else {
    qreal minLeg = 0.28 * officialDist;

    if (a < minLeg || b < minLeg || c < minLeg) {
      // qDebug() << "small, invalid dimensions";
      return -1;
    }
  }

  return officialDist;
}

bool TaskData::isClosed() const {
  return size() > 2 && distance(0, size() - 1) < 500;
}

}  // End namespace Updraft
