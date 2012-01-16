#ifndef UPDRAFT_SRC_LIBRARIES_IGC_IGC_H_
#define UPDRAFT_SRC_LIBRARIES_IGC_IGC_H_

#include <QByteArray>
#include <QDate>
#include <QIODevice>
#include <QList>
#include <QString>
#include <QTextCodec>
#include <QTime>

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
class LIBIGC_EXPORT IgcFile {
 public:
  typedef QList<Event const*> EventList;
  typedef QListIterator<Event const*> EventListIterator;

  ~IgcFile() { clear(); }

  bool load(QString path, QTextCodec *codec = 0);
  bool load(QIODevice *file, QTextCodec *codec = 0);

  void clear();

  /// Return altimeter pressure setting in hectopascals or zero
  /// if it was not specified.
  /// This value doesn't affect altitudes returned in fixes in any way.
  /// All recorded altitudes use 1013.25 as a base pressure.
  qreal altimeterSetting() const { return altimeterSetting_; }

  /// Return competition class or null string.
  QString competitionClass() const { return competitionClass_; }

  /// Return glider competition ID or null string.
  QString competitionId() const { return competitionId_; }

  /// Return date of the recording.
  QDate date() const { return date_; }

  /// Return FR manufacturer or null string.
  QString manufacturer() const { return manufacturer_; }

  /// Return FR type or null string.
  QString frType() const { return frType_; }

  /// Return glider registration number or null string.
  QString gliderId() const { return gliderId_; }

  /// Return GPS receiver type or null string.
  QString gps() const { return gps_; }

  /// Return glider model or null string.
  QString gliderType() const { return gliderType_; }

  /// Return pilot name or null string.
  QString pilot() const { return pilot_; }

  /// Return a const reference to the event map.
  const EventList& events() const { return eventList; }

 private:
  bool isEndOfFile();

  void parseOneRecord();

  QTime parseTimestamp(QByteArray bytes);
  qreal parseLatLon(QByteArray bytes);
  qreal parseDecimal(QByteArray bytes);
  QDate parseDate(QByteArray bytes);

  void processRecordB();
  void processRecordH();
  void processRecordL();

  static bool eventLessThan(Event const* e1, Event const* e2);

  EventList eventList;

  QByteArray buffer;

  char previousRecord;
  QIODevice* file;
  QTextCodec *activeCodec;

  /// Data extracted from IGC headers.
  /// \{
  qreal altimeterSetting_;
  QString competitionClass_;
  QString competitionId_;
  QDate date_;
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

