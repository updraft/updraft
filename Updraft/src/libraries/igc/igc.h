#ifndef UPDRAFT_SRC_LIBRARIES_IGC_IGC_H_
#define UPDRAFT_SRC_LIBRARIES_IGC_IGC_H_

#include <QByteArray>
#include <QTime>
#include <QIODevice>
#include <QMultiMap>
#include <QString>
#include <QTextCodec>

#ifdef UPDRAFT_LIB_INTERNAL
  #define LIBIGC_EXPORT Q_DECL_EXPORT
#else
  #define LIBIGC_EXPORT Q_DECL_IMPORT
#endif

namespace Updraft {
namespace Igc {

/// A single event from the igc file.
/// the field type determines which subclass of Event
/// this is.
struct Event {
  enum EventType {
    FIX = 1,
    PILOT_EVENT = 2
  };

  EventType type;

  QTime timestamp;
};

/// GPS fix event.
struct Fix : public Event {
  qreal lat;
  qreal lon;
  bool valid;
  qreal pressureAlt;
  qreal gpsAlt;
};

/// Pilot event.
struct PilotEvent : public Event {};

/// A class that loads an IGC file.
class LIBIGC_EXPORT Igc {
 public:
  typedef QMultiMap<QTime, Event*> EventMap;
  typedef QMapIterator<QTime, Event*> EventMapIterator;

  ~Igc() { clear(); }

  bool load(QString path, QTextCodec *codec = 0);
  bool load(QIODevice *file, QTextCodec *codec = 0);

  void clear();

  /// Return altimeter pressure setting in hectopascals or zero
  /// if it was not specified.
  /// This value doesn't affect altitudes returned in fixes in any way.
  /// All recorded altitudes use 113.25 as a base pressure.
  qreal altimeterSetting() { return altimeterSetting_; }

  /// Return competition class or empty string.
  QString competitionClass() { return competitionClass_; }

  /// Return glider competition ID or empty string.
  QString competitionId() { return competitionId_; }

  /// Return FR manufacturer or empty string.
  QString manufacturer() { return manufacturer_; }

  /// Return FR type or empty string.
  QString frType() { return frType_; }

  /// Return glider registration number or empty string.
  QString gliderId() { return gliderId_; }

  /// Return GPS receiver type or empty string.
  QString gps() { return gps_; }

  /// Return glider model or empty string.
  QString gliderType() { return gliderType_; }

  /// Return pilot name or empty string.
  QString pilot() { return pilot_; }

  /// Return a const reference to the event map.
  const EventMap& events() { return eventMap; }

 private:
  bool isEndOfFile();

  void parseOneRecord();

  QTime parseTimestamp(QByteArray bytes);
  qreal parseLatLon(QByteArray bytes);
  qreal parseDecimal(QByteArray bytes);

  void processRecordB();
  void processRecordH();
  void processRecordL();

  EventMap eventMap;

  QByteArray buffer;

  char previousRecord;
  QIODevice* file;
  QTextCodec *activeCodec;

  /// Data extracted from IGC headers.
  /// \{
  qreal altimeterSetting_;
  QString competitionClass_;
  QString competitionId_;
  QString manufacturer_;
  QString frType_;
  QString gliderId_;
  QString gps_;
  QString gliderType_;
  QString pilot_;
  /// \}
};

}  // End namespace Igc
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_IGC_IGC_H_

