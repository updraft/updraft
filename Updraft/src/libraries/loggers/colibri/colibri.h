#ifndef UPDRAFT_SRC_LIBRARIES_LOGGERS_COLIBRI_COLIBRI_H_
#define UPDRAFT_SRC_LIBRARIES_LOGGERS_COLIBRI_COLIBRI_H_

#ifdef UPDRAFT_COLIBRI_INTERNAL
  #define COLIBRI_EXPORT Q_DECL_EXPORT
#else
  #define COLIBRI_EXPORT Q_DECL_IMPORT
#endif

#include <QMutex>
#include <QThread>

#include "../loggerinterface.h"

namespace Updraft {
namespace Colibri {

class ColibriThread;

/// Handles communication with the LX Colibri logger.
class Colibri: public QObject, public LoggerInterface {
  Q_OBJECT

 public:
  Colibri(QObject *parent = 0);
  ~Colibri();

  /// Attempt to start a connection to the logger.
  /// \param dev Opened IO device.
  /// \return Success flag.
  /// \note This function returning true means that calls to
  ///   ping() will return true as well.
  virtual bool open(QextSerialPort* dev);

  /// Close the connection to the logger.
  /// Multiple calls of this function should be safe.
  virtual void close();

  /// Return a list of IGC files on the logger.
  virtual QList<QString> listIgc();

  /// Write the igc file with index i to dev.
  /// \return Success flag.
  virtual bool retreiveIgc(unsigned i, QIODevice *dev);
 
 public slots:
  /// Test the connection status.
  /// If this function fails, we emit the connectionError() signal and
  /// close the connection.
  /// \return True if connection is OK, false otherwise.
  virtual bool ping();

 signals:
  void opened();
  void closed();
  void connectionError();

 private slots:
  void finalizeOpen();

 private:
  enum {
    COLIBRI_ECHO = 0x16,
    COLIBRI_ECHO_REPLY = 0x06,
  };

  /// How many retries  before we consider a ping failed.
  static const unsigned PING_RETRY_COUNT = 3;
  
  /// Handle connection error.
  /// Emits the connectionError signal and closes the connection.
  void error();

  /// Attempt a single ping.
  /// \return true if it was succesful, false otherwise.
  void ping_internal();

  QTimer timer;
};


/// Thread for all the asynchronous work in colibri communication.
class ColibriThread : public QThread {
 public:
  QextSerialPort *port;

};

}  // End namespace Core
}  // End namespace Updraft

#endif  // UPDRAFT_SRC_LIBRARIES_LOGGERS_COLIBRI_COLIBRI_H_

