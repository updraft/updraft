#ifndef UPDRAFT_SRC_LIBS_LIBIGC_IGC_H_
#define UPDRAFT_SRC_LIBS_LIBIGC_IGC_H_

#include <QByteArray>
#include <QTime>
#include <QIODevice>
#include <QMultiMap>
#include <QString>
#include <QTextCodec>

namespace Updraft {
namespace Libigc {

/// A single event from the igc file.
struct Event {
  /// Type of the event. Indicates what subclass of Event is used.
  char type;

  QTime timestamp;
};

/// GPS fix event. Type is 'B'.
struct Fix : public Event {
  qreal lat;
  qreal lon;
  char validity;
  qreal pressureAlt;
  qreal gpsAlt;
};

/// Pilot event. Type is 'P'.
struct PilotEvent : public Event {};

/// A class that loads an IGC file.
class Q_DECL_EXPORT Igc {
 public:
  bool load(QString path, QTextCodec *codec = 0);
  bool load(QIODevice *file, QTextCodec *codec = 0);

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

 private:
  class Event {
   public:
    QDateTime timestamp;
  };

  bool isEndOfFile();

  bool parseOneRecord();

  void processRecordB();
  void processRecordH();
  void processRecordL();

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

}  // End namespace LibIgc
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBS_LIBIGC_IGC_H_

