#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_GEOMETRY_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_GEOMETRY_H_

#include "openairspace_global.h"

namespace OpenAirspace {

/// Coordinates structure - e.g. 39:29.9 N -119:46.1 E.
/// \TODO: use Utils when available
struct OPENAIRSPACE_EXPORT Coordinate {
  bool valid;  // validity flag
  qreal lat;
  qreal lon;
};

typedef Coordinate Position;

class OPENAIRSPACE_EXPORT Geometry {
 public :
  /// Geometry types
  /// DA: Draw Arc type I
  /// DB: Draw arc type II
  /// DC: Draw circle
  /// DP: Draw polygon
  enum GType { DAtype, DBtype, DCtype, DPtype };
  virtual const Position& Centre() const = 0;
  virtual const GType GetGType() const = 0;
  virtual ~Geometry() {}
};

class OPENAIRSPACE_EXPORT Polygon : public Geometry {
 public:
  Polygon();
  Polygon(const Position& C_, const double& z_);
  inline const Position& Centre() const { return *this->centre; }
  inline const GType GetGType() const { return DPtype; }
  ~Polygon();
 private:
  Position* centre;
  double zoom;
  bool valid;
};

class OPENAIRSPACE_EXPORT ArcI : public Geometry {
 public:
  ArcI();
  ArcI(const Position& C_, const double& R_, const bool CW_,
    const double& S_, const double& E_, const double& Z_);
  inline const Position& Centre() const { return *this->centre; }
  inline const double& Start() const { return this->start; }
  inline const double& End() const { return this->end; }
  inline const bool CW() const { return this->cw; }
  inline const double R() const { return this->r; }
  inline const GType GetGType() const { return DAtype; }
  ~ArcI();
 private:
  Position* centre;
  double r;  // radius in nm
  bool cw;  // arc direction
  double start;  // start angle in deg
  double end;  // end angle in deg
  double zoom;
  bool valid;
};

class OPENAIRSPACE_EXPORT ArcII : public Geometry {
 public:
  ArcII();
  ArcII(const Position& c_, const Position& s_, const Position e_,
    const bool cw_, const double& z_);
  inline const Position& Centre() const { return *this->centre; }
  inline const Position& Start() const { return *this->start; }
  inline const Position& End() const { return *this->end; }
  inline const bool CW() const { return this->cw; }
  inline const GType GetGType() const { return DBtype; }
  ~ArcII();
 private:
  Position* centre;
  Position* start;
  Position* end;
  bool cw;
  double zoom;
  bool valid;
};

class OPENAIRSPACE_EXPORT Circle : public Geometry {
 public:
  Circle();
  Circle(const Position& c_, const double R_, const double& z_);
  inline const Position& Centre() const { return *this->centre; }
  inline const double& R() const { return this->r; }
  inline const GType GetGType() const { return DCtype; }
  ~Circle();
 private:
  Position* centre;
  double r;  // radius
  double zoom;
  bool valid;
};
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_GEOMETRY_H_
