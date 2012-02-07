#include "cup.h"

#include <QDebug>
#include <QFile>

namespace Updraft {
namespace Cup {

QString CupFile::getFileName() const {
  return fileName;
}

QList<TPEntry> CupFile::getTPEntries() const {
  return turnPoints;
}

void CupFile::setFileName(const QString &name) {
  fileName = name;
}

void CupFile::addTPEntry(const TPEntry &tp) {
  turnPoints.append(tp);
}

void CupFile::addTaskEntry(const TaskEntry &task) {
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
      if (strLine.startsWith("-----Related Tasks-----", Qt::CaseInsensitive)) {
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

void CupLoader::parseLatitude(TPEntry &tp, const QString &latitude) {
  int degs = latitude.mid(0, 2).toInt();
  int mins = latitude.mid(2, 2).toInt();
  int rest = latitude.mid(5, 3).toInt();
  char sign = latitude.mid(8, 1).compare("S") == 0 ? 'S' : 'N';
  tp.location.latFromDMS(degs, mins, 60.0*((qreal)rest)/1000.0, sign);
}

void CupLoader::parseLongitude(TPEntry &tp, const QString &longitude) {
  int degs = longitude.mid(0, 3).toInt();
  int mins = longitude.mid(3, 2).toInt();
  int rest = longitude.mid(6, 3).toInt();
  char sign = longitude.mid(9, 1).compare("W") == 0 ? 'W' : 'E';
  tp.location.lonFromDMS(degs, mins, 60.0*((qreal)rest)/1000.0, sign);
}

void CupLoader::parseElevation(TPEntry &tp, const QString &elevation) {
  if (elevation.endsWith("m", Qt::CaseInsensitive)) {
    tp.location.alt = elevation.left(elevation.length()-1).toDouble();
  } else if (elevation.endsWith("ft", Qt::CaseInsensitive)) {
    tp.location.alt = Util::Units::feetToMeters(
      elevation.left(elevation.length()-2).toDouble());
  }
}

void CupLoader::parseTP(const QString &strLine) {
  TPEntry tp;

  tp.name = strLine.section(QChar(','), 0, 0);
  tp.code = strLine.section(QChar(','), 1, 1);
  tp.country = strLine.section(QChar(','), 2, 2);
  parseLatitude(tp, strLine.section(QChar(','), 3, 3));
  parseLongitude(tp, strLine.section(QChar(','), 4, 4));
  parseElevation(tp, strLine.section(QChar(','), 5, 5));
  tp.style = strLine.section(QChar(','), 6, 6);
  tp.rwyDirection = strLine.section(QChar(','), 7, 7);
  tp.rwLength = strLine.section(QChar(','), 8, 8);
  tp.frequency = strLine.section(QChar(','), 9, 9);
  tp.description = strLine.section(QChar(','), 10, 10);

  cupFile->addTPEntry(tp);
}

void CupLoader::parseTask(const QString &strLine) {
  // TODO(Tom): Think wheter to load the tasks from cup.
}

}  // End namespace Cup
}  // End namespace Updraft
