#include "colibri.h"

#include <QDebug>

namespace Updraft {
namespace Colibri {

Colibri::Colibri(QObject *parent)
  : QObject(parent) {}

Colibri::~Colibri() {
  close();
}

bool Colibri::open(QextSerialPort *dev) {
  port = dev;

  port->open(QIODevice::ReadWrite);

  timer.stop();

  port->putChar(COLIBRI_ECHO);
  port->putChar(COLIBRI_ECHO);

  QTimer::singleShot(1000, this, SLOT(finalizeOpen()));
}

void Colibri::finalizeOpen() {
  if (ping()) {
    emit connected();

    connect(&timer, SIGNAL(timeout()), this, SLOT(ping));
    timer.start(1000);
  }
}

void Colibri::close() {
  timer.stop();
  port->close();
}

bool Colibri::ping() {
  for (i = 0; i < PING_RETRY_COUNT; ++i) {
    if (ping_internal()) {
      return true;
    }
  }

  error();
  return false;
}

bool Colibri::ping_internal() {
  port.flush();

  if (!port.putChar(COLIBRI_ECHO)) {
    return false
  }

  char c;
  if (!port.getChar(&c)) {
    return false;
  }

  if (c != COLIBRI_ECHO_REPLY) {
    return false;
  }

  return true;
}

void Colibri::error() {
  qDebug() << "Colibri connection error";
  emit connectionError();
  close();
}

}  // End namespace Colibri
}  // End namespace Updraft

