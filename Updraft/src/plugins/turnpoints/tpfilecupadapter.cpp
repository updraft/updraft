#include "tpfilecupadapter.h"

namespace Updraft {

TPFileCupAdapter::~TPFileCupAdapter() {
  delete cupFile;
}

QString TPFileCupAdapter::getFileName() const {
  return QFileInfo(cupFile->getFileName()).fileName();
}

QString TPFileCupAdapter::getFilePath() const {
  return cupFile->getFileName();
}

TTPList TPFileCupAdapter::getTurnPoints() const {
  return tpList;
}

TPFileCupAdapter* TPFileCupAdapter::load(const QString &filename) {
  Cup::CupLoader cupLoader;
  Cup::CupFile *file = cupLoader.loadFile(filename);

  if (file == NULL) {
    return NULL;
  }

  return new TPFileCupAdapter(file);
}

TPFileCupAdapter::TPFileCupAdapter(const Cup::CupFile *cup)
  : cupFile(cup) {
  // Load turn-points from cup file and convert them to struct TurnPoint.
  // This is a kind of caching. It's faster than converting on each
  // getTurnPoints call, but it assumes that file doesn't change.
  QList<Cup::TPEntry> tpEntries = cupFile->getTPEntries();

  foreach(Cup::TPEntry entry, tpEntries) {
    TurnPoint tp;
    convertCupTPEntry(&tp, entry);
    tpList.append(tp);
  }
}

void TPFileCupAdapter::convertCupTPEntry(TurnPoint *tp,
  const Cup::TPEntry &tpEntry) {
  tp->code        = tpEntry.code;
  tp->name        = tpEntry.name;
  tp->location    = tpEntry.location;
  tp->type        = (WaypointStyle)tpEntry.style.toInt();
  tp->rwyHeading  = tpEntry.rwyDirection.toInt();
  tp->rwyLengthM  = parseLength(tpEntry.rwLength);
  tp->airportFreq = tpEntry.frequency.toFloat();
}

float TPFileCupAdapter::parseLength(const QString& text) {
  // Parse the text to number
  int s = text.indexOf(QRegExp("[0-9]"));
  int e = text.lastIndexOf(QRegExp("[0-9]"));
  if (s >= e)
    return -1;
  float length = text.mid(s, e - s + 1).toFloat();

  // Get the units
  if        (text.contains("nm", Qt::CaseInsensitive)) {
    length *= Util::NM_TO_M;
  } else if (text.contains("ml", Qt::CaseInsensitive)) {
    length *= Util::ML_TO_M;
  } else if (text.contains("ft", Qt::CaseInsensitive)) {
    length *= Util::FT_TO_M;
  }
  return length;
}
}  // End namespace Updraft
