#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_

#include "airspace.h"

namespace OpenAirspace {

  /// Parsing OpenAirspace file into geometry.
  class OPENAIRSPACE_EXPORT Parser {
 public:
    /// OpenAirspace Parser Ctor
    /// Parses the gven OpenAirspace file
    /// \param fileName The file to be parsed
    explicit Parser(const QString& fileName);

    /// \return Airspace.
    inline Airspace* at(const int i) {
      return this->allAirspaces->at(i);}

    /// \return The name of the AirSpace
    inline const QString* const GetName(int i) const {
      return this->allAirspaces->at(i)->GetName();}

    /// \return The used airspaces count
    inline size_t size() {
      return (allAirspaces) ?
        this->allAirspaces->size() : 0;}

    /// \return The class of the AirSpace
    inline const Airspace::ACType& GetClass(int i) {
      return this->allAirspaces->at(i)->GetClass();}

    /// \return The floor of the AirSpace
    inline const QString* const GetFloor(int i) const {
      return this->allAirspaces->at(i)->GetFloor(); }

    /// \return The ceiling of the AirSpace
    inline const QString* const GetCeiling(int i) const {
      return this->allAirspaces->at(i)->GetCeiling(); }

    /// \return The tag coordinates of the AirSpace
    inline const
      QVector<OpenAirspace::Position*>& GetTagCoor(int i) {
      return this->allAirspaces->at(i)->GetTagCoor(); }

    /// The class destructor.
    ~Parser(void);

 private:
    /// OpenAirspace contains several airspaces.
    QVector<Airspace*>* allAirspaces;
  };  // Parser
}  // OpenAirspace
#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_OPENAIRSPACE_H_
