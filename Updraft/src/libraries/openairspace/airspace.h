// UserAirspace.h

#ifndef UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_AIRSPACE_H_
#define UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_AIRSPACE_H_

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

namespace OpenAirspace {


  /// Coordinates structure - e.g. 39:29.9 N -119:46.1 E.
  /// \TODO: use Utils when available
  struct OPENAIRSPACE_EXPORT Coordinate {
    bool valid;  // validity flag
    qreal lat;
    qreal lon;
  };

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
    enum ACType { R, Q, P, A, B, C, D, E, GP, CTR, W };

    /// Geometry types
    /// DA: Draw Arc type I
    /// DB: Draw arc type II
    /// DC: Draw circle
    /// DP: Draw polygon
    enum GType { DAtype, DBtype, DCtype, DPtype };

    /// Coordinates structure - e.g. 39:29.9 N -119:46.1 E.
    /// \TODO: use Utils when available
    struct Coordinate {
      bool valid;  // validity flag
      qreal lat;
      qreal lon;
    };

    /// Polygon point structure
    struct Poly {
      GType type;
      Coordinate* coor;
    };

    /// Arc structures
    struct ArcI {
      GType type;
      bool valid;
      unsigned int R;  // radius in nm
      Coordinate* Centre;  // centre of the arc
      bool CW;  // clockwise flag
      unsigned int Start;  // start of the arc in deg
      unsigned int End;  // end of the arc in deg
      float Zoom;  // zoom /TODO: float vs int
    };
    struct ArcII {
      GType type;
      bool valid;
      Coordinate* Centre;  // centre of the arc
      Coordinate* Start;  // Starting coord
      Coordinate* End;  // Ending coord
      bool CW;  // direction
      float Zoom;  // zoom /TODO: float vs int
    };

    /// Circle structure
    struct Circle {
      GType type;
      bool valid;
      unsigned int R;  // radius in nm
      Coordinate* Centre;  // Centre of the circle in N E
      float Zoom;  // zoom /TODO: float vs int
    };

    /// Pen style structure
    struct SP_str {
      int style;
      int width;
      int R, G, B;
    };

    /// Brush colour selector
    struct SB_str {
      int R, G, B;
    };

    /// UserAirspace class constructor code.
    /// This takes the filename in Userirspace free format and parses
    /// the data contained into private variables
    explicit Airspace(QTextStream* ts);

    /// Returns the name of the AirSpace
    inline const QString& GetName() { return *this->AN; }

    /// Returns the class of the AirSpace
    inline const OpenAirspace::Airspace::ACType& GetClass() { return this->AC; }

    /// Returns the floor of the AirSpace
    inline const QString& GetFloor() { return *this->AL; }

    /// Returns the ceiling of the AirSpace
    inline const QString& GetCeiling() { return *this->AH; }

    /// Returns the tag coordinates of the AirSpace
    inline const QList<OpenAirspace::Airspace::Coordinate*>& GetTagCoor() {
      return *this->AT; }

    /// Returns the arcs type I
    inline const QList<OpenAirspace::Airspace::ArcI*>& GetArcI() {
      return *this->DA; }

    /// Returns the arcs type I
    inline const QList<OpenAirspace::Airspace::ArcII*>& GetArcII() {
      return *this->DB; }

    /// Returns the circles
    inline const QList<OpenAirspace::Airspace::Circle*>& GetCircle() {
      return *this->DC; }

    /// Returns the AirWay
    inline const QList<OpenAirspace::Airspace::Coordinate*>& GetAirWay() {
      return *this->DY; }

    /// Returns the Polygon
    inline const QList<OpenAirspace::Airspace::Coordinate*>& GetPolygon() {
      return *this->DP; }

    /// Returns the Polygon point
    inline const OpenAirspace::Airspace::Coordinate& GetPolygon(int i) {
      return *this->DP->at(i); }

    /// Returns the geometry element
    // inline const QPair<OpenAirspace::Airspace::ACType, void*>&
      // GetGeometry(int i) { return this->geometry->at(i); }

    // inline const void* GetGeometry(int i) { return this->geometry->at(i); }
    inline const QList<void*>& GetGeometry() { return *this->geometry; }

    /// Get Pen & Brush
    inline const SP_str* GetPen() {
      if (this->validSP)
        return SP;
      else
        return NULL; }
    inline const SB_str* GetBrush() {
      if (this->validSB)
        return SB;
      else
        return NULL; }

    /// UserAirspace destructor code here.
    Airspace::~Airspace();

    /// Coordinates of the center used by more airspaces.
    static Coordinate X;
    static bool validX;

  private :
    /// Parse the coordinates from string
    Coordinate* ParseCoord(const QString& parse);

    /// Airspace class type.
    ACType AC;

    /// Airspace Name.
    /// Contains the name of the aispace.
    /// Required.
    QString* AN;
    bool validAN;

    /// Airspace Floor.
    /// Altitude floor of the airspace in ft.
    /// Use -10000 for SFC.
    QString* AL;
    bool validAL;

    /// Airspace Ceiling
    /// Altitude ciling of the airspace in ft.
    /// AH < AL
    QString* AH;
    bool validAH;

    /// Airspace name label coordinates.
    /// List of coordinates, where to place the airspace name label on the map.
    /// Optional.
    QList<Coordinate*>* AT;
    bool validAT;

    /// Terrain related variables \{
    /// Terrain open polygon name
    QString* TO;
    bool validTO;

    /// Terrain closed polygon
    QString* TC;
    bool validTC;

    /// Pen & Brush var
    SP_str* SP;
    bool validSP;
    SB_str* SB;
    bool validSB;

    /// \}
    /// Record type - terrain & airspace \{
    /// Arc direction.
    bool CW;

    /// Airway width in nm. -1 for not set.
    int Wi;

    /// Zoom level.
    /// Sets the zoom level at which the airspace become visible.
    float Z;
    bool validZ;

    /// Add polygon points
    QList<Coordinate*>* DP;
    bool validDP;

    /// Arc \{

    /// Arcs in airspace
    QList<ArcI*>* DA;
    QList<ArcII*>* DB;
    bool validDA;
    bool validDB;
    /// \}

    /// Circles in the airspace. \{

    /// Cisrcles in airspace
    QList<Circle*>* DC;
    /// \}

    /// Airway
    QList<Coordinate*>* DY;
    bool validDY;
    /// \}

    /// Array of geometry
    // QList< QPair<GType, void*> >* geometry;
    QList<void*>* geometry;
    bool validG;
  };  // Airspace

/*
  class OPENAIRSPACE_EXPORT Geometry {
  public :
    virtual Coordinate* coordinate;
    virtual ~Geomeetry() {}
  }

  class OPENAIRSPACE_EXPORT Poly : Geometry {
  }

  class OPENAIRSPACE_EXPORT ArcI : Geometry {
  }

  class OPENAIRSPACE_EXPORT ArcII : Geometry {
  }

  class OPENAIRSPACE_EXPORT Circle : Geometry {
  }

*/
}  // OpenAirspace


#endif  // UPDRAFT_SRC_LIBRARIES_OPENAIRSPACE_AIRSPACE_H_
