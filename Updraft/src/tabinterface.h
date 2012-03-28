#ifndef UPDRAFT_SRC_TABINTERFACE_H_
#define UPDRAFT_SRC_TABINTERFACE_H_

class QObject;

namespace Updraft {

/// An interface wrapping a single tab in bottom pane.
class TabInterface {
 public:
  virtual ~TabInterface() {}

  /// Connect to the close(void) slot of tab.
  /// After this slot is called signal closed is emited
  /// and pointer to this becomes invalid.
  /// \see QObject::connect()
  virtual void connectSlotClose(const QObject* sender,
    const char *signal) = 0;

  /// Connect to a signal tabCloseRequested(void) that is emited before closing the tab.
  /// \see QObject::connect()
  virtual void connectSignalCloseRequested(const QObject* receiver,
    const char *method) = 0;

  /// Connect signal tabCloseRequested(void) to slot close.
  /// Call this method when the automatic closing is desired.
  /// \see QObject::connect()
  virtual void connectCloseRequestToClose() = 0;

  /// Connects to a signal selected(void) -- tab gets focus.
  /// \see QObject::connect()
  virtual void connectSignalSelected(const QObject* receiver,
    const char *method) = 0;

  /// Connects to a signal selected(void) -- tab loses focus.
  /// \see QObject::connect()
  virtual void connectSignalDeselected(const QObject* receiver,
    const char *method) = 0;

  /// Close this tab.
  /// This method deletes the tab and its widget.
  virtual void close() = 0;

  /// Select this tab in the tab pane.
  virtual void select() = 0;
};

}  // End namespace Updraft

#endif  // UPDRAFT_SRC_TABINTERFACE_H_

