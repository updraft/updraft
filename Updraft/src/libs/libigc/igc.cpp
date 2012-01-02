#include "igc.h"

#include <QDebug>
#include <QFile>

namespace Updraft {
namespace LibIgc {

/// Open a file with given path and
/// load it.
bool Igc::load(QString path, QTextCodec* codec) {
  QFile f(path);
  
  if (!f.open(QIODevice::ReadOnly)) {
    qDebug() << "Couldn't open " << path << ".";
    return false;
  }

  return load(&f, codec);
}

/// Load a file from opened QIODevice.
bool Igc::load(QIODevice *dev, QTextCodec* codec) {
  file = dev;
  previousRecord = '\0';

  altimeterSetting_ = 0;
  competitionClass_ = "";
  competitionId_ = "";
  manufacturer_ = "";
  frType_ = "";
  gliderId_ = "";
  gps_ = "";
  gliderType_ = "";
  pilot_ = "";

  if (codec) {
    activeCodec = codec;
  } else {
    activeCodec = QTextCodec::CodecForName("Latin1");
  }
 
  while (!isEndOfFile()) {
    buffer = file->readLine();

    if (buffer.size() == 0) {
      qDebug() << "Error reading file (" << file->errorString() << ")";
      return false;
    }

    parseOneRecord();

    previousRecord = buffer[0];
  }
  return true;
}

/// Parse a single record stored in the buffer.
void Igc::parseOneRecord() {
  switch (buffer[0]) {
    case 'A':
      /* Do nothing */
      break;
    case 'B':
      /* Do nothing */
      break;
    case 'H':
      processRecordH();
      break;
    case 'D':
      /* Do nothing */
      break;
    case 'C':
      /* Do nothing */
      break;
    case 'I':
      /* Do nothing */
      break;
    case 'J':
      /* Do nothing */
      break;
    case 'B':
      /* Do nothing */
      processRecordB();
      break;
    case 'K':
      /* Do nothing */
      break;
    case 'L':
      processRecordL();
      break;
    case 'G':
      /* Do nothing */
      break;
    default:
      qDebug() << "Unrecognized record '" << buffer[0] << "'.";
  }
}

/// Fid out if the current IGC file has ended.
/// This happens either if the file ends, or after the last G record.
bool Igc::isEndOfFile() {
  if (file->atEnd()) {
    return true;
  }

  if (previousRecord != 'G') {
    return false;
  }

  bufferUsed = file->peek(buffer, 1);
  if (bufferUsed != 1) {
    // There was a problem with peeking.
    // Now we're saying everything is OK, but the problem should show up later.
    return false;
  }

  if (buffer[0] != 'G') {
    return true;
  }
}

/// Process a single record of type B (fix data) stored in buffer.
void Igc::processRecordB() {
  Fix* ret = new Fix;

  ret->type = 'B';

  ret->timestamp = parse_timestamp(buffer.mid(1, 6));
  ret->lat = parse_lat(buffer.mid(7, 8));
  ret->lon = parse_lon(buffer.mid(15, 9));
  ret->valid = buffer[24];
  ret->pressureAlt = parse_decimal(buffer.mid(25, 5));
  ret->gpsAlt = parse_decimal(buffer.mid(30, 5));
}

/// Process a single record of type H (headers) stored in buffer.
void Igc::processRecordH() {
  //char dataSource = buffer[1];
  QByteArray subtype = buffer.mid(2, 3);
  QByteArray data = buffer.mid(5);

  int colonPos = data.indexOf(':');
  QByteArray value;

  if (colonPos != -1) {
    QByteArray value = data.mid(colonPos)
  }

  if (subtype == "ATS") {
    altimeterSetting_ = parse_decimal(value);
  } else if (subtype == "CCL") {
    competitionClass_ = activeCodec->toUnicode(value);
  } else if (subtype == "CID") {
    competitionId_ = activeCodec->toUnicode(value);
  } else if (subtype == "DTM") {
    if (data.left(3) != "100") {
      qDebug() << "We only support WGS84!";
    }
  } else if (subtype == "FTY") {
    QList<QByteArray> list = value.split(',')
    manufacturer_ = activeCodec->toUnicode(lis[0]);
    frType_ = activeCodec->toUnicode(lis[1]);
    gliderId_ = activeCodec->toUnicode(value);
  } else if (subtype == "GID") {
    gliderId_ = activeCodec->toUnicode(value);
  } else if (subtype == "GPS") {
    gps_ = activeCodec->toUnicode(value.split(',')[0])
  } else if (subtype == "GTY") {
    gliderType_ = activeCodec->toUnicode(value);
  } else if (subtype == "PLT") {
    pilot_ = activeCodec->toUnicode(value);
  }
}

/// Process a single record of type L (comments) stored in buffer.
void Igc::processRecordL() {
  if (buffer.mid(1, 4) == "CU::") {
    // This is a special seeyou comment.
    // Causes the rest of line to be read as a new record.
    // Used for saving values from user interface (security record
    // disregard L records)
	  buffer = buffer.mid(5);
    parseOneRecord();
  }
}

}  // End namespace Core
}  // End namespace Updraft

