#ifndef UPDRAFT_SRC_CORE_ELLIPSOIDNAME_H_
#define UPDRAFT_SRC_CORE_ELLIPSOIDNAME_H_

#include <QString>
#include <QMetaType>

namespace Updraft {
namespace Core {

/// Helper class representing name of an ellipsoid model.
/// Needed to get a specialized editor for plain QString.
class EllipsoidName: public QString {
 public:
  EllipsoidName();
  explicit EllipsoidName(const QString &str);

  QString asQString() const;
};

}  // End namespace Core
}  // End namespace Updraft

Q_DECLARE_METATYPE(Updraft::Core::EllipsoidName)

#endif  // UPDRAFT_SRC_CORE_ELLIPSOIDNAME_H_

