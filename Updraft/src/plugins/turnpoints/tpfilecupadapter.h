#ifndef UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPFILECUPADAPTER_H_
#define UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPFILECUPADAPTER_H_

#include <QtGui>
#include <QString>
#include "../../libraries/cup/cup.h"
#include "tpfile.h"
#include "../../libraries/util/units.h"

namespace Updraft {

/// Adapter for cup turn-points file
/// Use this class for loading turn-points from cup file.
class TPFileCupAdapter : public TPFile {
 public:
  virtual ~TPFileCupAdapter();

  // Implementation of TPFile interface.
  QString getFileName() const;
  QString getFilePath() const;
  TTPList getTurnPoints() const;

  /// Loads turn-points from specified cup file.
  /// Use this method for creating TPFileCupAdapter instances.
  /// \param filename full path of a cup file
  /// \return Pointer to TPFileCupAdapter on success, NULL on fail.
  static TPFileCupAdapter* load(const QString &filename);

 private:
  const Cup::CupFile *cupFile;
  TTPList tpList;

  /// Disallows direct construction and copying from outside.
  explicit TPFileCupAdapter(const Cup::CupFile *cup);
  TPFileCupAdapter(const TPFileCupAdapter&) {}

  /// Converts cup file turn-point entry into struct TurnPoint.
  /// \param [out] tp pointer to destinatin TurnPoint instance
  /// \param tpEntry entry of cup file for conversion
  void convertCupTPEntry(TurnPoint *tp, const Cup::TPEntry &tpEntry);

  /// Parses the string containing the length with units
  /// to float in meters
  float parseLength(const QString& text);
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_PLUGINS_TURNPOINTS_TPFILECUPADAPTER_H_
