#ifndef UPDRAFT_SRC_LIBRARIES_UTIL_ELLIPSOID_H_
#define UPDRAFT_SRC_LIBRARIES_UTIL_ELLIPSOID_H_

#include "util.h"

namespace osg {
  class EllipsoidModel;
}

namespace GeographicLib {
  class Geodesic;
}

namespace Updraft {
namespace Util {

class Location;

/// Ellipsoid model of Earth
/// It encapsulates (contains) osg::EllipsoidModel
/// and capabilities of GeographicLib::Geodesic.
class UTIL_EXPORT Ellipsoid {
 public:
  Ellipsoid(const QString &name_, qreal eqRadius, qreal flattening_);

  /// Create sphere.
  Ellipsoid(const QString &name_, qreal radius);

  virtual ~Ellipsoid();

  /// \return Name of ellipsoid
  QString getName() const;

  /// \return Equatorial radius of ellipsoid
  qreal getEquatRadius() const;

  /// \return Polar radius of ellipsoid
  qreal getPolarRadius() const;

  /// \return Flattening of ellipsoid
  qreal getFlattening() const;

  const osg::EllipsoidModel* getOsgEllipsoidModel() const;

  /// Counts the shortest distance between two geographical points.
  /// \param l1 first point of measured segment
  /// \param l2 second point of measured segment
  /// \return Distance in meters between l1 and l2.
  qreal distance(const Location &l1, const Location &l2) const;

  /// Counts the shortest distance and corresponding azimuths
  /// between two geographical points.
  /// \param l1 first point of measured segment
  /// \param l2 second point of measured segment
  /// \param a12 [out] pointer to azimuth from l1 to l2. Can be NULL.
  /// \param a21 [out] pointer to azimuth from l2 to l1. Can be NULL.
  /// \return Distance in meters between l1 and l2.
  qreal distanceAzimuth(const Location &l1, const Location &l2,
    qreal *a12, qreal *a21) const;

  /// Counts flattening from equatorial radius and polar radius.
  /// \param rE equatorial radius
  /// \param rP polar radius
  static qreal countFlattening(qreal rE, qreal rP);

  /// Counts polar radius from equatorial radius and flattening.
  /// \param r equatorial radius
  /// \param f flattening
  static qreal countPolarRadius(qreal r, qreal f);

 protected:
  QString name;
  qreal equatRadius;
  qreal flattening;

  /// Osg object representing this ellipsoid
  osg::EllipsoidModel *osgEllipsoidModel;

  /// Object for counting geodesics (shortest paths)
  GeographicLib::Geodesic *geodesic;
};

}  // End namespace Util
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_UTIL_ELLIPSOID_H_
