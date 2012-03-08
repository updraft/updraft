#ifndef UPDRAFT_SRC_LIBRARIES_LOGGERS_LOGGERINTERFACE_H_
#define UPDRAFT_SRC_LIBRARIES_LOGGERS_LOGGERINTERFACE_H_

#include <QList>
#include <QString>

#include <qextserialport.h>

class QIODevice;

namespace Updraft {

/// Interface to communication with flight loggers.
class LoggerInterface {
 public:

  /// Destructor.
  /// Should calls close() internaly.
  ~LoggerInterface() {}

  /// Attempt to start a connection to the logger.
  /// \param dev Serial port device. This has to be set up correctly, logger
  ///   class will only attempt to call open() method and start communication.
  virtual void open(QextSerialPort *dev) = 0;

  /// Close the connection to the logger.
  /// Multiple calls of this function should be safe.
  virtual void close() = 0;

  /// Test the connection status.
  /// \return True if connection is OK, false otherwise.
  virtual bool ping() = 0;

  /// Return a list of IGC files on the logger.
  virtual QList<QString> listIgc() = 0;

  /// Write the igc file with index i to dev.
  /// \return Success flag.
  virtual bool retreiveIgc(unsigned i, QIODevice *dev) = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_LOGGERS_LOGGERINTERFACE_H_

