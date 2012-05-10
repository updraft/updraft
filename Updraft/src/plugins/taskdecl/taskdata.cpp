#include <math.h>

#include <QDebug>
#include <QtXml/QDomDocument>
#include <osg/CoordinateSystemNode>

#include "taskdata.h"
#include "taskpoint.h"
#include "util/util.h"
#include "pluginbase.h"

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

bool TaskData::insertTaskPoint(TaskPoint *taskPoint, int position) {
  if (taskPoint == NULL) {
    return false;
  }

  if (position < 0 || position >= taskPoints.size()) {
    position = taskPoints.size();
  }

  Util::Location loc1 = taskPoint->getLocation();

  // Prevent inserting TaskPoints that are too close to each other
  if (position != 0) {
    TaskPoint* prev = taskPoints[position - 1];
    Util::Location loc2 = prev->getLocation();
    // TODO(cestmir): Put the tollerance into settings
    if (g_core->getEllipsoid()->distance(loc1, loc2) < 500) {
      return false;
    }
  }

  if (position != taskPoints.size()) {
    TaskPoint* next = taskPoints[position];
    Util::Location loc2 = next->getLocation();
    if (g_core->getEllipsoid()->distance(loc1, loc2) < 500) {
      return false;
    }
  }

  taskPoints.insert(position, taskPoint);

  // The newly selected addTaskPoint Button will be the one after the inserted
  // TaskPoint. That means it will have the new TaskPoint's position + 1
  selectedAddButton = position + 1;
  return true;
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

  // We want to set the selected add button to the deleted segment.
  selectedAddButton = position;
}

void TaskData::setAddTaskPointButton(int position) {
  if (position < -1 || position > taskPoints.size()) {
    return;
  }

  selectedAddButton = position;
}

int TaskData::getAddTaskPointButton() const {
  return selectedAddButton;
}

TaskData::TaskData(const TaskData& taskData) {
  // Deep copy of taskPoints container
  foreach(TaskPoint *point, taskData.taskPoints) {
    taskPoints.push_back(new TaskPoint(*point));
  }

  selectedAddButton = taskData.selectedAddButton;
}

bool TaskData::isFaiTriangle() const {
  return faiTriangleDistance() >= 0;
}

qreal TaskData::distance(int i, int j) const {
  Util::Location loc1 = taskPoints[i]->getLocation();
  Util::Location loc2 = taskPoints[j]->getLocation();

  return g_core->getEllipsoid()->distance(loc1, loc2);
}

qreal TaskData::distanceAzimuth(int i, int j, qreal *azimuth) const {
  Util::Location loc1 = taskPoints[i]->getLocation();
  Util::Location loc2 = taskPoints[j]->getLocation();

  return g_core->getEllipsoid()->distanceAzimuth(loc1, loc2, azimuth);
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
  } else if (size() == 5) {
    a = distance(1, 2);
    b = distance(2, 3);
    c = distance(3, 1);
  } else {
    // qDebug() << "Not a triangle";
    return -1;
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
