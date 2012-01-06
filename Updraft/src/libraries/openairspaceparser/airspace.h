// UserAirspace.h

#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACEPARSER_AIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACEPARSER_AIRSPACE_H_

#include "openairspace_global.h"

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

namespace Updraft {

  class Airspace {
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
    enum ACType { R, Q, P, A, B, C, D, GP, CTR, W };

    /// Coordinates structure - e.g. 39:29.9 N -119:46.1 E.
    struct Coordinate {
      bool valid;  // validity flag
      struct ECor {
        int min;
        float sec;
      };
      ECor N;
      ECor E;
    };

    /// Arc structures
    struct ArcI {
      bool valid;
      unsigned int R;  // radius in nm
      Coordinate Centre;  // centre of the arc
      bool CW;  // clockwise flag
      unsigned int Start;  // start of the arc in deg
      unsigned int End;  // end of the arc in deg
      float Zoom;  // zoom /TODO: float vs int
    };
    struct ArcII {
      bool valid;
      Coordinate Centre;  // centre of the arc
      Coordinate Start;  // Starting coord
      Coordinate End;  // Ending coord
      bool CW;  // direction
      float Zoom;  // zoom /TODO: float vs int
    };

    /// Circle structure
    struct Circle {
      bool valid;
      unsigned int R;  // radius in nm
      Coordinate Centre;  // Centre of the circle in N E
      float Zoom;  // zoom /TODO: float vs int
    };

    /// Pen style structure
    struct SP_str {
      bool valid;
      int style;
      int width;
      int R, G, B;
    };

    /// Brush colour selector
    struct SB_str {
      bool valid;
      int R, G, B;
    };

    /// UserAirspace class constructor code.
    /// This takes the filename in Userirspace free format and parses
    /// the data contained into private variables
    explicit Airspace(QTextStream* ts);

    /// Returns the name of the AirSpace
    inline const QString& GetName() { return this->AN; }

    /// Returns the class of the AirSpace
    inline const int GetClass() { return this->AC; }

    /// Returns the floor of the AirSpace
    inline const QString& GetFloor() { return this->AL; }

    /// Returns the ceiling of the AirSpace
    inline const QString& GetCeiling() { return this->AH; }

    /// Returns the tag coordinates of the AirSpace
    inline const QList<Updraft::Airspace::Coordinate>& GetTagCoor() {
      return this->AT; }

    /// Returns the arcs type I
    inline const QList<Updraft::Airspace::ArcI>& GetArcI() {
      return this->DA; }

    /// Returns the arcs type I
    inline const QList<Updraft::Airspace::ArcII>& GetArcII() {
      return this->DB; }

    /// Returns the circles
    inline const QList<Updraft::Airspace::Circle>& GetCircle() {
      return this->DC; }

    /// Returns the AirWay
    inline const QList<Updraft::Airspace::Coordinate>& GetAirWay() {
      return this->DY; }


    /// UserAirspace destructor code here.
    // Airspace::~Airspace();

    /// Coordinates of the center used by more airspaces.
    static Coordinate X;

  private :
    /// Parse the coordinates from string
    Coordinate ParseCoord(const QString& parse);

    /// Airspace class type.
    ACType AC;

    /// Airspace Name.
    /// Contains the name of the aispace.
    /// Required.
    QString AN;
    bool validAN;

    /// Airspace Floor.
    /// Altitude floor of the airspace in ft.
    /// Use -10000 for SFC.
    QString AL;
    bool validAL;

    /// Airspace Ceiling
    /// Altitude ciling of the airspace in ft.
    /// AH < AL
    QString AH;
    bool validAH;

    /// Airspace name label coordinates.
    /// List of coordinates, where to place the airspace name label on the map.
    /// Optional.
    QList<Coordinate> AT;

    /// Terrain related variables \{
    /// Terrain open polygon name
    QString TO;
    bool validTO;

    /// Terrain closed polygon
    QString TC;
    bool validTC;

    /// Pen & Brush var
    SP_str SP;
    SB_str SB;

    /// \}
    /// Record type - terrain & airspace \{
    /// Centre of the polygon/circle.
    // static Coordinate X;
    bool validX;

    /// Arc direction.
    bool CW;

    /// Airway width in nm. -1 for not set.
    int Wi;

    /// Zoom level.
    /// Sets the zoom level at which the airspace become visible.
    float Z;
    bool validZ;

    /// Add polygon points
    QList<Coordinate> DP;

    /// Arc \{

    /// Arcs in airspace
    QList<ArcI> DA;
    QList<ArcII> DB;
    bool validDA;
    bool validDB;
    /// \}

    /// Circles in the airspace. \{

    /// Cisrcles in airspace
    QList<Circle> DC;
    /// \}

    /// Airway
    QList<Coordinate> DY;
    /// \}
  };  // UserAirspace
}  // Updraft


#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACEPARSER_AIRSPACE_H_
