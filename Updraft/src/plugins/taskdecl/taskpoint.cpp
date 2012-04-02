#include <QtXml/QDomDocument>
#include "taskpoint.h"
#include "plugins/turnpoints/turnpoint.h"

namespace Updraft {

TaskPoint::TaskPoint(const QDomElement &elem) {
  QDomElement areaElem = elem.firstChildElement("Area");

  if (!areaElem.isNull()) {
    QString str = areaElem.attribute("areaShape");

    if (str.compare("line", Qt::CaseInsensitive) == 0) {
      area.areaShape = AssignedArea::LINE;
    } else if (str.compare("sector", Qt::CaseInsensitive) == 0) {
      area.areaShape = AssignedArea::SECTOR;
    }

    str = areaElem.attribute("direction");

    if (str.compare("fixed", Qt::CaseInsensitive) == 0) {
      area.direction = AssignedArea::FIXED;
    } else if (str.compare("symmetrical", Qt::CaseInsensitive) == 0) {
      area.direction = AssignedArea::SYMMETRICAL;
    } else if (str.compare("to_next_point", Qt::CaseInsensitive) == 0) {
      area.direction = AssignedArea::TO_NEXT_POINT;
    } else if (str.compare("to_previous_point", Qt::CaseInsensitive) == 0) {
      area.direction = AssignedArea::TO_PREVIOUS_POINT;
    } else if (str.compare("to_start_point", Qt::CaseInsensitive) == 0) {
      area.direction = AssignedArea::TO_START_POINT;
    }

    area.maxRadius = areaElem.attribute("maxRadius", "0.5").toDouble();
    area.minRadius = areaElem.attribute("minRadius", "0.0").toDouble();
    area.outerAngle = areaElem.attribute("outerAngle", "0.0").toDouble();
    area.innerAngle = areaElem.attribute("innerAngle", "0.0").toDouble();
    area.fixedAngle = areaElem.attribute("fixedAngle", "0.0").toDouble();
  }

  QDomElement codeElem = elem.firstChildElement("Code");

  if (!codeElem.isNull()) {
    code = codeElem.text();
  }

  QDomElement nameElem = elem.firstChildElement("Name");

  if (!nameElem.isNull()) {
    name = nameElem.text();
  }

  QDomElement locationElem = elem.firstChildElement("Location");

  if (!locationElem.isNull()) {
    QDomElement latElem = locationElem.firstChildElement("Latitude");

    if (!latElem.isNull()) {
      qreal d, m, s;
      d = latElem.attribute("degrees", "0").toDouble();
      m = latElem.attribute("minutes", "0").toDouble();
      s = latElem.attribute("seconds", "0").toDouble();

      QString sign = latElem.attribute("sign", "N");
      location.latFromDMS(d, m, s,
        sign.length() > 0 ? sign[0].toAscii() : 'N');
    }

    QDomElement lonElem = locationElem.firstChildElement("Longitude");

    if (!lonElem.isNull()) {
      qreal d, m, s;
      d = lonElem.attribute("degrees", "0").toDouble();
      m = lonElem.attribute("minutes", "0").toDouble();
      s = lonElem.attribute("seconds", "0").toDouble();

      QString sign = lonElem.attribute("sign", "E");
      location.lonFromDMS(d, m, s,
        sign.length() > 0 ? sign[0].toAscii() : 'E');
    }
  }
}

void TaskPoint::toDom(QDomDocument *doc, QDomElement *taskPointElem) const {
  // Creates xml element for assigned area.
  QDomElement areaElem = doc->createElement("Area");

  // Set attributes of area xml element.

  switch (area.areaShape) {
    case AssignedArea::LINE:
      areaElem.setAttribute("areaShape", "line");
      break;

    case AssignedArea::SECTOR:
    default:
      areaElem.setAttribute("areaShape", "sector");
      break;
  }

  switch (area.direction) {
    case AssignedArea::FIXED:
      areaElem.setAttribute("direction", "fixed");
      break;

    case AssignedArea::SYMMETRICAL:
    default:
      areaElem.setAttribute("direction", "symmetrical");
      break;

    case AssignedArea::TO_NEXT_POINT:
      areaElem.setAttribute("direction", "to_next_point");
      break;

    case AssignedArea::TO_PREVIOUS_POINT:
      areaElem.setAttribute("direction", "to_previous_point");
      break;

    case AssignedArea::TO_START_POINT:
      areaElem.setAttribute("direction", "to_start_point");
      break;
  }

  areaElem.setAttribute("maxRadius", QString("%1").arg(area.maxRadius));
  areaElem.setAttribute("minRadius", QString("%1").arg(area.minRadius));
  areaElem.setAttribute("outerAngle", QString("%1").arg(area.outerAngle));
  areaElem.setAttribute("innerAngle", QString("%1").arg(area.innerAngle));
  areaElem.setAttribute("fixedAngle", QString("%1").arg(area.fixedAngle));

  taskPointElem->appendChild(areaElem);

  // Creates xml element for code.
  if (code.length() > 0) {
    QDomElement codeElem = doc->createElement("Code");
    codeElem.setNodeValue(code);
    taskPointElem->appendChild(codeElem);
  }

  // Creates xml element for name.
  if (name.length() > 0) {
    QDomElement nameElem = doc->createElement("Name");
    nameElem.setNodeValue(name);
    taskPointElem->appendChild(nameElem);
  }

  // Creates xml element for location.
  QDomElement locElem = doc->createElement("Location");
  taskPointElem->appendChild(locElem);

  // Creates xml element for latitude.
  QDomElement latElem = doc->createElement("Latitude");
  locElem.appendChild(latElem);

  char sign;
  int degs, mins;
  qreal secs;
  location.latToDMS(&degs, &mins, &secs, &sign);

  latElem.setAttribute("sign", QString(sign));
  latElem.setAttribute("degrees", QString("%1").arg(degs));
  latElem.setAttribute("minutes", QString("%1").arg(mins));
  latElem.setAttribute("seconds", QString("%1").arg(secs));

  // Creates xml element for longitude.
  QDomElement lonElem = doc->createElement("Longitude");
  locElem.appendChild(lonElem);

  location.lonToDMS(&degs, &mins, &secs, &sign);

  lonElem.setAttribute("sign", QString(sign));
  lonElem.setAttribute("degrees", QString("%1").arg(degs));
  lonElem.setAttribute("minutes", QString("%1").arg(mins));
  lonElem.setAttribute("seconds", QString("%1").arg(secs));
}

QString TaskPoint::getCode() const {
  return code;
}

QString TaskPoint::getName() const {
  return name;
}

const Util::Location& TaskPoint::getLocation() const {
  return location;
}

const AssignedArea& TaskPoint::getAssignedArea() const {
  return area;
}

void TaskPoint::setTP(const TurnPoint *tp) {
  if (tp == NULL) {
    return;
  }

  code = tp->code;
  name = tp->name;
  location = tp->location;
}

void TaskPoint::setLocation(const Util::Location &location_) {
  location = location_;
}

void TaskPoint::setAssignedArea(const AssignedArea &area_) {
  area = area_;
}

}  // End namespace Updraft
