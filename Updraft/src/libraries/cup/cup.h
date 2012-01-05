#ifndef UPDRAFT_SRC_LIBRARIES_CUP_CUP_H_
#define UPDRAFT_SRC_LIBRARIES_CUP_CUP_H_

#include <QtCore/QtGlobal>

#ifdef CUP_LIB
# define CUP_EXPORT Q_DECL_EXPORT
#else
# define CUP_EXPORT Q_DECL_IMPORT
#endif

namespace Cup {

/// Stores and encapsulates data of the SeeYou cup file.
/// Cup file is loaded by the CupLoader.
/// Provides access to the loaded data.
class CUP_EXPORT CupFile {
};

/// Performs loading of SeeYou cup files.
/// Creates CupFile instances.
class CUP_EXPORT CupLoader {
 public:
  /// Loads file from disk.
  /// \param name a name of the file which is to be loaded
  /// \return Pointer to the new CupFile instance
  CupFile* loadFile(const QString &name);
};
}  // End namespace Cup

#endif  // UPDRAFT_SRC_LIBRARIES_CUP_CUP_H_
