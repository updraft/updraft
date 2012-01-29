#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_

#include "airspace.h"

namespace OpenAirspace {
  class OPENAIRSPACE_EXPORT Parser {
 public:
    /// OpenAirspace Parser Ctor
    /// Parses the \param OpenAirspace file
    explicit Parser(const QString&);

    /// Returns the name of the AirSpace
    inline const QString& GetName(int i) {
      return this->allAirspaces[i].GetName();}

    /// Returns the used airspaces count
    inline size_t size() {
      return this->allAirspaces.size(); }

    /// Returns the class of the AirSpace
    inline const QString& GetClass(int i) {
      return this->allAirspaces[i].GetClass();}

    /// Returns the floor of the AirSpace
    inline const QString& GetFloor(int i) {
      return this->allAirspaces[i].GetFloor(); }

    /// Returns the ceiling of the AirSpace
    inline const QString& GetCeiling(int i) {
      return this->allAirspaces[i].GetCeiling(); }

    /// Returns the tag coordinates of the AirSpace
    inline const
      QList<OpenAirspace::Airspace::Coordinate>& GetTagCoor(int i) {
      return this->allAirspaces[i].GetTagCoor(); }

    /// Returns the arcs type I
    inline const
      QList<OpenAirspace::Airspace::ArcI>& GetArcI(int i) {
      return this->allAirspaces[i].GetArcI(); }

    /// Returns the arcs type I
    inline const
      QList<OpenAirspace::Airspace::ArcII>& GetArcII(int i) {
      return this->allAirspaces[i].GetArcII(); }

    /// Returns the circles
    inline const
      QList<OpenAirspace::Airspace::Circle>& GetCircle(int i) {
      return this->allAirspaces[i].GetCircle(); }

    /// Returns the AirWay
    inline const
      QList<OpenAirspace::Airspace::Coordinate>& GetAirWay(int i) {
      return this->allAirspaces[i].GetAirWay(); }

    ~Parser(void);

 private:
    /// OpenAirspace contains several airspaces.
    QList<Airspace> allAirspaces;
  };  // Parser
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
