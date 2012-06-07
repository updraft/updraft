// UserAirspace.h

#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_AIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_AIRSPACE_H_

#include "openairspace_global.h"
#include "geometry.h"


/*!
UserAirspace parser library.

******* OPEN AIR (tm) TERRAIN and AIRSPACE DESCRIPTION LANGUAGE *************
*    Version 1.0
*    December 10, 1998
*    Updated October 15, 1999
*    Send comments to jerryp@winpilot.com
*
*
*  AIRSPACE related record types:
*  ==============================
*
* AC class    ;    class = Airspace Class, see below:
*     R restricted
*     Q danger
*     P prohibited
*     A Class A
*     B Class B
*     C Class C
*     D Class D 
*     GP glider prohibited 
*     CTR CTR
*     W Wave Window
*
*
*  AN string        ;     string = Airspace Name
*  AH string        ;     string = Airspace Ceiling
*  AL string        ;     string = Airspace Floor
*  AT coordinate    ;    coordinate = Coordinate of where to place a name label on the map (optional)
*                     ;     NOTE: there can be multiple AT records for a single airspace segment
*    
*    
*    TERRAIN related record types (WinPilot version 1.130 and newer):
*    ==============================
*
*    TO    {string}                 ; Declares Terrain Open Polygon; string = name (optional)
*    TC    {string}                 ; Declares Terrain Closed Polygon; string = name (optional)
*    SP style, width, red, green, blue    ; Selects Pen to be used in drawing
*    SB red, green, blue                         ; Selects Brush to be used in drawing
*
*
*    Record types common to both TERRAIN and AIRSPACE
*    =================================================
*
*    V x=n             ;     Variable assignment.
*                     ;     Currently the following variables are supported:
*                     ;     D={+|-}    sets direction for: DA and DB records
*                     ;                     '-' means counterclockwise direction; '+' is the default
*                     ;                     automatically reset to '+' at the begining of new airspace segment    
*                     ;     X=coordinate    : sets the center for the following records: DA, DB, and DC    
*                     ;     W=number        : sets the width of an airway in nm (NYI)
*                     ;      Z=number         : sets zoom level at which the element becomes visible (WP version 1.130 and newer)
*
*    DP coordinate                     ; add polygon pointC
*    DA radius, angleStart, angleEnd    ; add an arc, angles in degrees, radius in nm (set center using V X=...)
*    DB coordinate1, coordinate2         ; add an arc, from coordinate1 to coordinate2 (set center using V X=...)
*    DC radius                         ; draw a circle (center taken from the previous V X=...  record, radius in nm
*    DY coordinate                     ; add a segment of an airway (NYI)



*/

namespace OpenAirspace {

/// This class represents a single
/// airspace in the OpenAir file.
class OPENAIRSPACE_EXPORT Airspace {
 public :
    /// Airspace class type enum.
    /*!   R restricted
    *     Q danger
    *     P prohibited
    *     A Class A
    *     B Class B
    *     C Class C
    *     D Class D 
    *     GP glider prohibited 
    *     CTR CTR
    *     W Wave Window */
    enum ACType { R, Q, P, A, B, C, D, E, GP, CTR, W, NA };

    /// Pen style structure.
    struct SP_str {
      int style;
      int width;
      int R, G, B;
    };

    /// Brush colour selector structure.
    struct SB_str {
      int R, G, B;
    };

    /// UserAirspace class constructor code.
    /// Parses the single airspace record.
    /// \param ts This takes the QTexttream in UseAir free
    /// format.
    Airspace(QTextStream* ts, bool* acOn);

    /// \return The name of the AirSpace.
    inline const QString* const GetName() const { return this->AN; }

    /// \return The class of the AirSpace
    inline const OpenAirspace::Airspace::ACType& GetClass() {
      return this->AC;
    }
    inline const QString& GetClassName() {
      if (!this->ACstring)
        this->ACstring = new QString("NA");
      return *this->ACstring;
    }

    /// \return The height of the floor of the Airspace.
    inline const QString* const GetFloor() const { return this->AL; }

    /// \return The height of the ceiling of the Airspace.
    inline const QString* const GetCeiling() const { return this->AH; }

    /// \return The tag coordinates of the Airspace.
    inline const QVector<Position*>& GetTagCoor() {
      return *this->AT; }

    /// \return The geometry array size.
    inline int GetGeometrySize() {
      return (geometry) ? this->geometry->size() : 0; }

    /// \return the geometry array (a primitive).
    inline const QVector<OpenAirspace::Geometry*>& GetGeometry() {
      return *this->geometry; }

    /// \return The AirWay.
    inline const QVector<Position*>& GetAirWay() {
      return *this->DY; }

    /// \return Pen.
    inline const SP_str* GetPen() const {
      if (this->SP)
        return SP;
      else
        return NULL; }
    /// \return Brush.
    inline const SB_str* GetBrush() const {
      if (this->SB)
        return SB;
      else
        return NULL; }

    /// UserAirspace destructor code here.
    ~Airspace();

    /// Coordinates of the center used by more airspaces.
    static Position X;
    static bool validX;

    /// Parse the height data from the text to feet.
    /// \param floor This parameter tells the routine
    /// whether it is processing the floor data.
    /// \param agl Returns true if the parsed height is
    /// above the ground level or absolute.
    /// \return The elevation in feet.
    int ParseHeight(bool floor, bool* agl);

  private :
    /// Parses the WGS coordinates from string.
    /// \param parse The string from which the coordinates
    /// should be parsed.
    /// \return The coordinates in Possition structure.
    Position ParseCoord(const QString& parse);

    /// Airspace class type.
    ACType AC;
    QString* ACstring;

    /// Airspace Name.
    /// Contains the name of the aispace.
    /// Required.
    QString* AN;

    /// Airspace Floor.
    /// Altitude floor of the airspace in ft.
    /// Use -10000 for SFC.
    QString* AL;

    /// Airspace Ceiling
    /// Altitude ciling of the airspace in ft.
    /// AH < AL
    QString* AH;

    /// Airspace name label coordinates.
    /// List of coordinates, where to place the airspace name label on the map.
    /// Optional.
    QVector<Position*>* AT;

    /// Terrain related variables \{
    /// Terrain open polygon name
    QString* TO;

    /// Terrain closed polygon
    QString* TC;

    /// Pen & Brush var
    SP_str* SP;
    SB_str* SB;

    /// \}
    /// Record type - terrain & airspace \{
    /// Arc direction.
    bool CW;

    /// Airway width in nm. -1 for not set.
    double Wi;

    /// Zoom level.
    /// Sets the zoom level at which the airspace become visible.
    float Z;

    /// Airway
    QVector<Position*>* DY;
    /// \}

    /// Array of geometry
    QVector<Geometry*>* geometry;
  };  // Airspace
}  // OpenAirspace


#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_AIRSPACE_H_
