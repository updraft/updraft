#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_

#include "airspace.h"

namespace OpenAirspace {

  /// Parsing OpenAirspace file into geometry.
  class OPENAIRSPACE_EXPORT Parser {
 public:
    /// OpenAirspace Parser Ctor
    /// Parses the \param OpenAirspace file
    explicit Parser(const QString&);

    /// Returns Airspace
    inline Airspace* at(const int i) {
      return this->allAirspaces->at(i);}

    /// Returns the name of the AirSpace
    inline const QString* const GetName(int i) const {
      return this->allAirspaces->at(i)->GetName();}

    /// Returns the used airspaces count
    inline size_t size() {
      return (allAirspaces) ?
        this->allAirspaces->size() : 0;}

    /// Returns the class of the AirSpace
    inline const Airspace::ACType& GetClass(int i) {
      return this->allAirspaces->at(i)->GetClass();}

    /// Returns the floor of the AirSpace
    inline const QString* const GetFloor(int i) const {
      return this->allAirspaces->at(i)->GetFloor(); }

    /// Returns the ceiling of the AirSpace
    inline const QString* const GetCeiling(int i) const {
      return this->allAirspaces->at(i)->GetCeiling(); }

    /// Returns the tag coordinates of the AirSpace
    inline const
      QVector<OpenAirspace::Position*>& GetTagCoor(int i) {
      return this->allAirspaces->at(i)->GetTagCoor(); }
/* NA
    /// Returns the arcs type I
    inline const
      QVector<Airspace::ArcI*>& GetArcI(int i) {
      return this->allAirspaces->at(i)->GetArcI(); }

    /// Returns the arcs type I
    inline const
      QVector<Airspace::ArcII*>& GetArcII(int i) {
      return this->allAirspaces->at(i)->GetArcII(); }

    /// Returns the circles
    inline const
      QVector<Airspace::Circle*>& GetCircle(int i) {
      return this->allAirspaces->at(i)->GetCircle(); }

    /// Returns the AirWay
    inline const
      QVector<OpenAirspace::Position*>& GetAirWay(int i) {
      return this->allAirspaces->at(i)->GetAirWay(); }
*/

    ~Parser(void);

 private:
    /// OpenAirspace contains several airspaces.
    QVector<Airspace*>* allAirspaces;
  };  // Parser
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
