#include "openairspace.h"

#include <QDebug>
#include <QFile>
#include <QString>


namespace OpenAirspace {
  Parser::Parser(const QString& fileName) {
    // qDebug("Parser ctor");
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly)) {
      qDebug("OpenAirspace file not found : ");
      return;
    }

    QTextStream ts(&file);
    QString text("");

    while (text != "AC" && !ts.atEnd())
      ts >> text;

    if (ts.atEnd())
      qDebug("Not supported OpenAirspace format.");
    else
      // qDebug("Parsing OpenAirspace file %s", fileName.toAscii().data());

    this->allAirspaces = new QList<Airspace*>();
    while (!ts.atEnd()) {
      if (!ts.atEnd())
        ts.seek(ts.pos() -3);
      Airspace* nextairspace = new Airspace(&ts);
      this->allAirspaces->push_back(nextairspace);
    }

    file.close();
  }
  Parser::~Parser(void) {
    // qDebug("Parser dtor");
    for (int i = 0; i < allAirspaces->size(); ++i) {
      Airspace* a = allAirspaces->at(i);
      delete a;
    }
    delete allAirspaces;
    allAirspaces = NULL;
  }
}  // OpenAirspace
