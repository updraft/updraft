#include "openairspace.h"

namespace Updraft {

  void OpenAirspace::ParseFile(const QString& fileName) {
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly))
      return;

    QTextStream ts(&file);
    QString text("");

    while (text != "AC" && !ts.atEnd())
      ts >> text;

    while (!ts.atEnd()) {
      if (!ts.atEnd())
        ts.seek(ts.pos() -3);
      Airspace nextairspace(&ts);
      this->allAirspaces.push_back(nextairspace);
    }

    file.close();
  }  // OpenAirspace
}  // Updraft
