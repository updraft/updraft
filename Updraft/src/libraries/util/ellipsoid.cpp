#include <osg/CoordinateSystemNode>
#include "GeographicLib/Geodesic.hpp"
#include "ellipsoid.h"

namespace Updraft {
namespace Util {

Ellipsoid::Ellipsoid(const QString& name_, EllipsoidType type_)
  : type(type_), name(name_) {
  switch (type_) {
    case ELLIPSOID_WGS84:
      equatRadius = Util::Units::WGS84EquatRadius();
      flattening  = Util::Units::WGS84Flattening();
    break;
    case ELLIPSOID_FAI_SPHERE:
      equatRadius = Util::Units::FAISphereRadius();
      flattening = 0.0;
    break;
  }

  geodesic = new GeographicLib::Geodesic(equatRadius, flattening);
  osgEllipsoidModel = new osg::EllipsoidModel(equatRadius, getPolarRadius());
}

Ellipsoid::~Ellipsoid() {
  delete geodesic;
  delete osgEllipsoidModel;
}

QString Ellipsoid::getName() const {
  return name;
}

EllipsoidType Ellipsoid::getType() const {
  return type;
}

QString Ellipsoid::getEllipsoidTypeName(EllipsoidType type) {
  switch (type) {
    case ELLIPSOID_FAI_SPHERE:
      return QString("FAI Sphere");
    break;
    default:
    case ELLIPSOID_WGS84:
      return QString("WGS84");
    break;
  }
}

EllipsoidType getEllipsoidType(const QString& typeName) {
  if (typeName == "WGS84") {
    return ELLIPSOID_WGS84;
  } else if (typeName == "FAI Sphere") {
    return ELLIPSOID_FAI_SPHERE;
  }

  // WGS84 will be default
  return ELLIPSOID_WGS84;
}

qreal Ellipsoid::getEquatRadius() const {
  return equatRadius;
}

qreal Ellipsoid::getPolarRadius() const {
  return countPolarRadius(equatRadius, flattening);
}

qreal Ellipsoid::getFlattening() const {
  return flattening;
}

const osg::EllipsoidModel* Ellipsoid::getOsgEllipsoidModel() const {
  return osgEllipsoidModel;
}

qreal Ellipsoid::countFlattening(qreal rE, qreal rP) {
  return 1.0 - rP / rE;
}

qreal Ellipsoid::countPolarRadius(qreal r, qreal f) {
  return r * (1.0 - f);
}

qreal Ellipsoid::distance(const Location &l1, const Location &l2) const {
  GeographicLib::Math::real s12;

  // Counts shortest distance, using GeographicLib.
  geodesic->Inverse(l1.lat, l1.lon, l2.lat, l2.lon, s12);
  return (qreal)s12;
}

qreal Ellipsoid::distanceAzimuth(const Location &l1, const Location &l2,
  qreal *azimuth) const {
  GeographicLib::Math::real s12, azi1, azi2;

  // Counts shortest distance and corresponding azimuths, using GeographicLib.
  geodesic->Inverse(l1.lat, l1.lon, l2.lat, l2.lon, s12, azi1, azi2);

  if (azimuth != NULL) {
    azi1 = azi1 < 0.0 ? azi1 + 360.0 : azi1;
    *azimuth = (qreal)azi1;
  }

  return (qreal)s12;
}

}  // End namespace Util
}  // End namespace Updraft
