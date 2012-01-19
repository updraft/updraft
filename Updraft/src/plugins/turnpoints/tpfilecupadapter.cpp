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
  tp->code = tpEntry.code;
  tp->name = tpEntry.name;
}

}  // End namespace Updraft
