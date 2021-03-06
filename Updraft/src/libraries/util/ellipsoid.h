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

enum EllipsoidType {
  ELLIPSOID_WGS84,
  ELLIPSOID_FAI_SPHERE
};

class Location;

/// Ellipsoid model of Earth
/// It encapsulates (contains) osg::EllipsoidModel
/// and capabilities of GeographicLib::Geodesic.
class UTIL_EXPORT Ellipsoid {
 public:
  Ellipsoid(const QString &name_, EllipsoidType type_);

  virtual ~Ellipsoid();

  /// \return Name of ellipsoid
  QString getName() const;

  /// \return The type of this ellipsoid
  EllipsoidType getType() const;

  /// \return The ellipsoid type represented as a string
  static QString getEllipsoidTypeName(EllipsoidType type);

  /// \return The ellipsoid type corresponding to the given name
  EllipsoidType getEllipsoidType(const QString& typeName);

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
  /// \param azimuth [out] pointer to azimuth at l1. Can be NULL.
  /// \return Distance in meters between l1 and l2.
  qreal distanceAzimuth(const Location &l1, const Location &l2,
    qreal *azimuth) const;

  /// Counts flattening from equatorial radius and polar radius.
  /// \param rE equatorial radius
  /// \param rP polar radius
  static qreal countFlattening(qreal rE, qreal rP);

  /// Counts polar radius from equatorial radius and flattening.
  /// \param r equatorial radius
  /// \param f flattening
  static qreal countPolarRadius(qreal r, qreal f);

 protected:
  EllipsoidType type;
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
