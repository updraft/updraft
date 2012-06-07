#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_GEOMETRY_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_GEOMETRY_H_

#include "openairspace_global.h"

namespace OpenAirspace {

/// Geographic coordinates.
struct OPENAIRSPACE_EXPORT Coordinate {
  // TODO(Kuba) use Utils::Location
  bool valid;  /// < validity flag
  qreal lat;
  qreal lon;
};

typedef Coordinate Position;

/// Abstract geometry in airspace.
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

/// Polygon class describes one polygon.
class OPENAIRSPACE_EXPORT Polygon : public Geometry {
 public:
  /// Default constructor.
  Polygon();

  /// Constructor.
  /// \param C_ The position to be copied.
  /// \param z_ The zoom level the element is visible.
  Polygon(const Position& C_, const double& z_);

  /// \return The Centre of the polygon.
  inline const Position& Centre() const { return *this->centre; }

  /// \return The type of the polygon.
  inline const GType GetGType() const { return DPtype; }

  /// Destructor
  ~Polygon();

 private:
  /// Centre of the polygon.
  Position* centre;

  /// Zoom level the element is visible.
  double zoom;

  /// Validity flag.
  bool valid;
};

/// Arc spcified by center point radius, start angle and end angle.
class OPENAIRSPACE_EXPORT ArcI : public Geometry {
 public:
  /// Default constructor.
  ArcI();

  /// The arc constructor. Creates the arc element.
  /// \param C_ The centre of the element.
  /// \param R_ The radius of the arc.
  /// \param CW_ The clockwise flag.
  /// \param S_ The starting angle of the arc.
  /// \param E_ The ending angle of the arc.
  /// \param Z_ The zoom level the element is visible.
  ArcI(const Position& C_, const double& R_, const bool CW_,
    const double& S_, const double& E_, const double& Z_);

  /// \return The centre of the arc.
  inline const Position& Centre() const { return *this->centre; }

  /// \return The start angle of the arc.
  inline const double& Start() const { return this->start; }

  /// \return The end angle of the arc.
  inline const double& End() const { return this->end; }

  /// \return The orientation (clockwise flag)
  inline const bool CW() const { return this->cw; }

  /// \return The radius of the arc.
  inline const double R() const { return this->r; }

  /// \return The type of the arc.
  inline const GType GetGType() const { return DAtype; }

  /// Destructor
  ~ArcI();

 private:
  /// Centre of the arc.
  Position* centre;

  /// Radius in nm
  double r;

  /// Arc direction.
  bool cw;

  /// Start angle in deg.
  double start;

  /// End angle in deg.
  double end;

  /// Zoom level the element is visible.
  double zoom;

  /// Validity flag.
  bool valid;
};

/// Arc spcified by center point, start point and end point.
class OPENAIRSPACE_EXPORT ArcII : public Geometry {
 public:
  /// Default constructor.
  ArcII();

  /// The arc constructor. Creates the arc element.
  /// \param C_ The centre of the element.
  /// \param S_ The starting point of the arc.
  /// \param E_ The end point of the arc.
  /// \param CW_ The clackwise flag (drawing direction).
  /// \param Z_ The zoom level the element is visible.
  ArcII(const Position& C_, const Position& S_, const Position E_,
    const bool CW_, const double& Z_);

  /// \return The centre of the arc.
  inline const Position& Centre() const { return *this->centre; }

  /// \return The starting point of the arc.
  inline const Position& Start() const { return *this->start; }

  /// \return The ending point of the arc.
  inline const Position& End() const { return *this->end; }

  /// \return The clockwise flag (direction of drawing).
  inline const bool CW() const { return this->cw; }

  /// \return The type of the arc.
  inline const GType GetGType() const { return DBtype; }

  /// The class destructor.
  ~ArcII();

 private:
  /// Centre of the arc.
  Position* centre;

  /// Startig point of the arc.
  Position* start;

  /// Ending point of the arc.
  Position* end;

  /// Clockwise flag.
  bool cw;

  /// Zoom level the element is visible.
  double zoom;

  /// Validity flag.
  bool valid;
};

/// Full circle class.
class OPENAIRSPACE_EXPORT Circle : public Geometry {
 public:
  /// default contructor
  Circle();

  /// The circle constructr construct a circle from the params.
  /// \param C_ The centre of the corcle.
  /// \param R_ The radius of the circle.
  /// \param Z_ The zoom the element is visible.
  Circle(const Position& C_, const double R_, const double& Z_);

  /// \retun The Centre point of the corcle.
  inline const Position& Centre() const { return *this->centre; }

  /// \return The radius of the circle.
  inline const double& R() const { return this->r; }

  /// \return The Type of the element.
  inline const GType GetGType() const { return DCtype; }

  /// Destructor.
  ~Circle();

 private:
  /// The centre of the circle.
  Position* centre;

  /// The radius of the circle.
  double r;

  /// The zoom the element is visible.
  double zoom;

  /// Validity flag.
  bool valid;
};
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_GEOMETRY_H_
