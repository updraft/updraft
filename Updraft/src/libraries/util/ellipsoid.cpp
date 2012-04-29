#include <osg/CoordinateSystemNode>
#include "GeographicLib/Geodesic.hpp"
#include "ellipsoid.h"

namespace Updraft {
namespace Util {

Ellipsoid::Ellipsoid(const QString &name_, qreal eqRadius, qreal flattening_)
  : name(name_), equatRadius(eqRadius), flattening(flattening_) {
  geodesic = new GeographicLib::Geodesic(equatRadius, flattening);
  osgEllipsoidModel = new osg::EllipsoidModel(equatRadius, getPolarRadius());
}

Ellipsoid::Ellipsoid(const QString &name_, qreal radius)
  : name(name_), equatRadius(radius), flattening(0.0) {
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
  return distanceAzimuth(l1, l2, NULL, NULL);
}

qreal Ellipsoid::distanceAzimuth(const Location &l1, const Location &l2,
  qreal *a12, qreal *a21) const {
  GeographicLib::Math::real s12, azi12, azi21;

  // Counts shortest distance and corresponding azimuths, using GeographicLib.
  geodesic->Inverse(l1.lat, l1.lon, l2.lat, l2.lon, s12, azi12, azi21);

  if (a12 != NULL) {
    *a12 = (qreal)azi12;
  }

  if (a21 != NULL) {
    *a21 = (qreal)azi21;
  }

  return (qreal)s12;
}

}  // End namespace Util
}  // End namespace Updraft
