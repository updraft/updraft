#ifndef OPENAIRSPACE_H
#define OPENAIRSPACE_H

#include "openairspace_global.h"

/*!
OpenAirspace parser library.

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

	/// delete
	class myf
	{public:
		//static __declspec(dllimport) int back();
	static __declspec(dllexport) double Add(double a, double b);
	};
	class OPENAIRSPACE_EXPORT OpenAirspace
	{
	public :
		/// OpenAirspace class constructor code. 
		/// This takes the filename in Userirspace free format and parses
		/// the data contained into private variables
		OpenAirspace::OpenAirspace(QString fileName);

		/// Returns the name of the AirSpace
		inline const QString& OpenAirspace::GetName(int i) {return this->allAirspaces[i].GetName();}

		/// Returns the used airspaces count
		inline size_t OpenAirspace::size() { return this->allAirspaces.size(); }

		/// Returns the class of the AirSpace
		inline const int OpenAirspace::GetClass(int i) {return this->allAirspaces[i].GetClass();}

		/// Returns the floor of the AirSpace
		inline const QString& OpenAirspace::GetFloor(int i) { return this->allAirspaces[i].GetFloor(); };

		/// Returns the ceiling of the AirSpace
		inline const QString& OpenAirspace::GetCeiling(int i) { return this->allAirspaces[i].GetCeiling(); };

		/// Returns the tag coordinates of the AirSpace
		inline const QList<Updraft::Airspace::Coordinate>& OpenAirspace::GetTagCoor(int i) { return this->allAirspaces[i].GetTagCoor(); };

		/// Returns the arcs type I
		inline const QList<Updraft::Airspace::ArcI>& OpenAirspace::GetArcI(int i) { return this->allAirspaces[i].GetArcI(); }
		
		/// Returns the arcs type I
		inline const QList<Updraft::Airspace::ArcII>& OpenAirspace::GetArcII(int i) { return this->allAirspaces[i].GetArcII(); }
		
		/// Returns the circles 
		inline const QList<Updraft::Airspace::Circle>& OpenAirspace::GetCircle(int i) { return this->allAirspaces[i].GetCircle(); }
		
		/// Returns the AirWay 
		inline const QList<Updraft::Airspace::Coordinate>& OpenAirspace::GetAirWay(int i) { return this->allAirspaces[i].GetAirWay(); }
		

		/// OpenAirspace destructor code here.
		OpenAirspace::~OpenAirspace();

	private :
		/// OpenAirspace contains several airspaces.
		QList<Airspace> allAirspaces;

	};// OpenAirspace

} //Updraft

#endif // OPENAIRSPACE_H
