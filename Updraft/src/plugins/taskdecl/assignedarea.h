#ifndef UPDRAFT_SRC_PLUGINS_TASKDECL_ASSIGNEDAREA_H_
#define UPDRAFT_SRC_PLUGINS_TASKDECL_ASSIGNEDAREA_H_

#include <QtGlobal>

namespace Updraft {

/// A structure defining area shape
/// Shape styles are same as in SeeYou 4.2 by NavIter
struct AssignedArea {
  /// Assigned area shapes.
  /// Other shapes like cylinder or FAI cylinder are special
  /// cases of SECTOR shape.
  /// E.g. FAI cylinder has maxRadius=0.5 outerAngle=180
  /// and other values set to 0.
  enum AreaShape {
    LINE,
    SECTOR
  };

  /// Definition of base direction with respect to two neighbour points
  /// or start point or fixed value.
  enum Direction {
    FIXED,
    SYMMETRICAL,
    TO_NEXT_POINT,
    TO_PREVIOUS_POINT,
    TO_START_POINT
  };

  /// Area shape type
  AreaShape areaShape;

  /// Base direction of the area
  Direction direction;

  /// Radius defining outer circle (maximum distance)
  /// Unit is km
  qreal maxRadius;

  /// Radius defining inner circle (minimal distance)
  /// Unit is km
  qreal minRadius;

  /// Angle from base direction to left and right,
  /// defining the range of outer sector. (range is 2*outerAngle)
  qreal outerAngle;

  /// Angle from base direction to left and right,
  /// defining the range of inner sector. (range is 2*innerAngle)
  qreal innerAngle;

  /// Angle which is used when the direction is FIXED
  qreal fixedAngle;

  /// Default constructor sets area to FAI cylinder.
  AssignedArea() : areaShape(SECTOR), direction(SYMMETRICAL), maxRadius(0.5),
    minRadius(0.0), outerAngle(180.0), innerAngle(0.0), fixedAngle(0.0) { }
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TASKDECL_ASSIGNEDAREA_H_
