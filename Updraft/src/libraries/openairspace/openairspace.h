#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_

#include "airspace.h"

namespace OpenAirspace {
  class OPENAIRSPACE_EXPORT Parser {
 public:
    /// OpenAirspace Parser Ctor
    /// Parses the \param OpenAirspace file
    explicit Parser(const QString&);

    /// Returns Airspace
    inline const Airspace& at(const int i) {
      return *this->allAirspaces->at(i);}

    /// Returns the name of the AirSpace
    inline const QString& GetName(int i) {
      return this->allAirspaces->at(i)->GetName();}

    /// Returns the used airspaces count
    inline size_t size() {
      return this->allAirspaces->size(); }

    /// Returns the class of the AirSpace
    inline const OpenAirspace::Airspace::ACType& GetClass(int i) {
      return this->allAirspaces->at(i)->GetClass();}

    /// Returns the floor of the AirSpace
    inline const QString& GetFloor(int i) {
      return this->allAirspaces->at(i)->GetFloor(); }

    /// Returns the ceiling of the AirSpace
    inline const QString& GetCeiling(int i) {
      return this->allAirspaces->at(i)->GetCeiling(); }

    /// Returns the tag coordinates of the AirSpace
    inline const
      QList<OpenAirspace::Airspace::Coordinate*>& GetTagCoor(int i) {
      return this->allAirspaces->at(i)->GetTagCoor(); }

    /// Returns the arcs type I
    inline const
      QList<OpenAirspace::Airspace::ArcI*>& GetArcI(int i) {
      return this->allAirspaces->at(i)->GetArcI(); }

    /// Returns the arcs type I
    inline const
      QList<OpenAirspace::Airspace::ArcII*>& GetArcII(int i) {
      return this->allAirspaces->at(i)->GetArcII(); }

    /// Returns the circles
    inline const
      QList<OpenAirspace::Airspace::Circle*>& GetCircle(int i) {
      return this->allAirspaces->at(i)->GetCircle(); }

    /// Returns the AirWay
    inline const
      QList<OpenAirspace::Airspace::Coordinate*>& GetAirWay(int i) {
      return this->allAirspaces->at(i)->GetAirWay(); }

    ~Parser(void);

 private:
    /// OpenAirspace contains several airspaces.
    QList<Airspace*>* allAirspaces;
  };  // Parser
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
