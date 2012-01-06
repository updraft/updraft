#include "cup.h"

#include <QDebug>
#include <QFile>

namespace Cup {

const QString& CupFile::getFileName() const {
  return fileName;
}

void CupFile::setFileName(const QString &name) {
  fileName = name;
}

void CupFile::addTP(const TurnPoint &tp) {
  turnPoints.append(tp);
}

void CupFile::addTask(const Task &task) {
  tasks.append(task);
}

CupLoader::CupLoader()
  : state(UNDEFINED), cupFile(NULL) {
}

CupFile* CupLoader::loadFile(const QString &name) {
  QFile file(name);

    // Open file
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "CupLoader: Couldn't open " << name << ".";
    return NULL;
  }

  cupFile = new CupFile();
  cupFile->setFileName(name);

    // Switch to initial state
  state = LOADING_HEADER;

    // Read file line by line
  while (!file.atEnd()) {
    QString strLine(file.readLine());

    if (strLine.length() == 0) {
      qDebug() << "CupLoader: Error reading file ("
        << file.errorString() << ")";
      delete cupFile;
      return NULL;
    }

    parseLine(strLine);

    if (state == ERROR) {
      qDebug() << "CupLoader: Error parsing file " << name;
      delete cupFile;
      return NULL;
    }
  }

  state = DONE;
  return cupFile;
}

void CupLoader::parseLine(const QString &strLine) {
  switch (state) {
    case LOADING_HEADER:
      state = LOADING_TPS;
      break;

    case LOADING_TPS:
      if (strLine.startsWith("-----Related Tasks-----")) {
        state = LOADING_TASKS;
        break;
      }

      parseTP(strLine);
      break;

    case LOADING_TASKS:
      parseTask(strLine);
      break;

    default:
      break;
  }
}

void CupLoader::parseTP(const QString &strLine) {
  TurnPoint tp;

  tp.name = strLine.section(QChar(','), 0, 0);
  tp.code = strLine.section(QChar(','), 1, 1);
  tp.country = strLine.section(QChar(','), 2, 2);
  tp.latitude = strLine.section(QChar(','), 3, 3);
  tp.longitude = strLine.section(QChar(','), 4, 4);
  tp.elevation = strLine.section(QChar(','), 5, 5);
  tp.style = strLine.section(QChar(','), 6, 6);
  tp.rwyDirection = strLine.section(QChar(','), 7, 7);
  tp.rwLength = strLine.section(QChar(','), 8, 8);
  tp.frequency = strLine.section(QChar(','), 9, 9);
  tp.description = strLine.section(QChar(','), 10, 10);

  cupFile->addTP(tp);
}

void CupLoader::parseTask(const QString &strLine) {
  // TODO(Tom): Think wheter to load the tasks from cup.
}

}  // End namespace Cup
