#include "igc.h"

#include <QtAlgorithms>
#include <QDebug>
#include <QFile>

namespace Updraft {
namespace Igc {

/// Open a file with given path and
/// load it.
bool IgcFile::load(QString path, QTextCodec* codec) {
  QFile f(path);

  if (!f.open(QIODevice::ReadOnly)) {
    qDebug() << "Couldn't open " << path << ".";
    return false;
  }

  return load(&f, codec);
}

/// Load a file from opened QIODevice.
bool IgcFile::load(QIODevice *dev, QTextCodec* codec) {
  clear();

  file = dev;
  previousRecord = '\0';

  if (codec) {
    activeCodec = codec;
  } else {
    activeCodec = QTextCodec::codecForName("Latin1");
  }

  while (!isEndOfFile()) {
    buffer = file->readLine().trimmed();

    if (buffer.size() == 0 && !file->atEnd()) {
      qDebug() << "Error reading file (" << file->errorString() << ")";
      return false;
    }

    parseOneRecord();

    previousRecord = buffer[0];
  }

  qSort(eventList.begin(), eventList.end(), eventLessThan);

  return true;
}

/// Delete all loaded data.
void IgcFile::clear() {
  altimeterSetting_ = 0;
  competitionClass_ = "";
  competitionId_ = "";
  manufacturer_ = "";
  frType_ = "";
  gliderId_ = "";
  gps_ = "";
  gliderType_ = "";
  pilot_ = "";

  foreach(Event const* ev, eventList) {
    delete ev;
  }

  eventList.clear();
}

/// Parse a single record stored in the buffer.
void IgcFile::parseOneRecord() {
  switch (buffer[0]) {
    case 'A':
      /* Do nothing */
      break;
    case 'B':
      processRecordB();
      break;
    case 'C':
      /* Do nothing */
      break;
    case 'D':
      /* Do nothing */
      break;
    case 'E':
      /* Do nothing */
      break;
    case 'F':
      /* Do nothing */
      break;
    case 'G':
      /* Do nothing */
      break;
    case 'H':
      processRecordH();
      break;
    case 'I':
      /* Do nothing */
      break;
    case 'J':
      /* Do nothing */
      break;
    case 'K':
      /* Do nothing */
      break;
    case 'L':
      processRecordL();
      break;
  }
}

/// Fid out if the current IGC file has ended.
/// This happens either if the file ends, or after the last G record.
bool IgcFile::isEndOfFile() {
  if (file->atEnd()) {
    return true;
  }

  if (previousRecord != 'G') {
    return false;
  }

  buffer = file->peek(1);
  if (buffer.size() != 1) {
    // There was a problem with peeking.
    // Now we're saying everything is OK, but the problem should show up later.
    return false;
  }

  if (buffer[0] != 'G') {
    return true;
  }

  return false;
}

/// Parse time from IGC encoding.
/// HHMMSS
QTime IgcFile::parseTimestamp(QByteArray bytes) {
  Q_ASSERT(bytes.size() == 6);
  int h = bytes.mid(0, 2).toInt();
  int m = bytes.mid(2, 2).toInt();
  int s = bytes.mid(4, 2).toInt();
  return QTime(h, m, s);
}

/// Parse latitude or longitude from IGC encoding.
/// \return Degrees. Negative values go south and west.
/// DDMMmmm[NS] or DDDMMmmm[EW]
qreal IgcFile::parseLatLon(QByteArray bytes) {
  Q_ASSERT(bytes.size() == 8 || bytes.size() == 9);

  int degreesSize = (bytes.size() == 8) ? 2 : 3;

  int d = bytes.mid(0, degreesSize).toInt();
  int m = bytes.mid(degreesSize, 2).toInt();
  int mDecimal = bytes.mid(degreesSize + 2, 3).toInt();

  qreal ret = d + m / 60.0 + mDecimal / 60000.0;

  char lastChar = bytes[bytes.size() - 1];
  if (lastChar == 'S' || lastChar == 'W') {
    return -ret;
  } else {
    return ret;
  }
}

/// Parse a decimal number in igc format.
qreal IgcFile::parseDecimal(QByteArray bytes) {
  int whole = bytes.left(bytes.size() - 2).toInt();
  int decimal = bytes.right(2).toInt();

  return whole + decimal / 100.0;
}

/// Parse date specification.
/// \bug Date field in igc files has only two digits for year.
///   Now we're just adding 2000 to it, but maybe there is some
///   smarter way around?
QDate IgcFile::parseDate(QByteArray bytes) {
  return QDate(
    bytes.mid(4, 2).toInt() + 2000,
    bytes.mid(2, 2).toInt(),
    bytes.mid(0, 2).toInt());
}

/// Process a single record of type B (fix data) stored in buffer.
void IgcFile::processRecordB() {
  Fix* ret = new Fix;

  ret->type = Event::FIX;

  ret->timestamp = parseTimestamp(buffer.mid(1, 6));
  ret->lat = parseLatLon(buffer.mid(7, 8));
  ret->lon = parseLatLon(buffer.mid(15, 9));
  ret->valid = (buffer[24] == 'A');
  ret->pressureAlt = buffer.mid(25, 5).toInt();
  ret->gpsAlt = buffer.mid(30, 5).toInt();

  eventList.append(ret);
}

/// Process a single record of type H (headers) stored in buffer.
void IgcFile::processRecordH() {
  // char dataSource = buffer[1];
  QByteArray subtype = buffer.mid(2, 3);
  QByteArray data = buffer.mid(5);

  int colonPos = data.indexOf(':');
  QByteArray value;

  if (colonPos != -1) {
    value = data.mid(colonPos + 1);
  }

  if (subtype == "ATS") {
    altimeterSetting_ = parseDecimal(value);
  } else if (subtype == "CCL") {
    competitionClass_ = activeCodec->toUnicode(value);
  } else if (subtype == "CID") {
    competitionId_ = activeCodec->toUnicode(value);
  } else if (subtype == "DTE") {
    date_ = parseDate(data);
  } else if (subtype == "DTM") {
    if (data.left(3) != "100") {
      qDebug() << "We only support WGS84!";
    }
  } else if (subtype == "FTY") {
    QList<QByteArray> list = value.split(',');
    manufacturer_ = activeCodec->toUnicode(list[0]);
    frType_ = activeCodec->toUnicode(list[1]);
    gliderId_ = activeCodec->toUnicode(value);
  } else if (subtype == "GID") {
    gliderId_ = activeCodec->toUnicode(value);
  } else if (subtype == "GPS") {
    gps_ = activeCodec->toUnicode(value.split(',')[0]);
  } else if (subtype == "GTY") {
    gliderType_ = activeCodec->toUnicode(value);
  } else if (subtype == "PLT") {
    pilot_ = activeCodec->toUnicode(value);
  }
}

/// Process a single record of type L (comments) stored in buffer.
void IgcFile::processRecordL() {
  if (buffer.mid(1, 4) == "CU::") {
    // This is a special seeyou comment.
    // Causes the rest of line to be read as a new record.
    // Used for saving values from user interface (security record
    // disregard L records)
    buffer = buffer.mid(5);
    parseOneRecord();
  }
}

/// Comparator function for sorting events in the list according to timestamp.
bool IgcFile::eventLessThan(Event const* e1, Event const* e2) {
  return e1->timestamp < e2->timestamp;
}

}  // End namespace Igc
}  // End namespace Updraft

