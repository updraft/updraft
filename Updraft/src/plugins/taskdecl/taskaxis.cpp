#include <QPushButton>
#include <QGridLayout>
#include <QPainter>
#include "taskaxis.h"
#include "taskfile.h"
#include "taskdata.h"
#include "taskpoint.h"
#include "taskdeclpanel.h"

namespace Updraft {

TaskAxis::TaskAxis(const TaskDeclPanel *panel, const TaskFile *taskFile_)
  : taskFile(taskFile_), taskDeclPanel(panel) {
}

void TaskAxis::taskDataChanged() {
  if (taskFile == NULL) {
    return;
  }
}

void TaskAxis::paintEvent(QPaintEvent *e) {
  QPainter painter(this);

  painter.setPen(QColor(0, 0, 0));
  painter.setBrush(QBrush(QColor(0, 0, 0)));
  QFontMetrics fontMetrics(painter.font());

  const TaskData *taskData = taskFile->beginRead();
  int y = height() / 2;

  // Draws empty task axis.
  if (taskData->size() <= 1) {
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.drawLine(0, y, width()-1, y);
  }

  for (int legIndex = 0; legIndex+1 < taskData->size(); ++legIndex) {
    // Gets widgets of corresponding taskpoints
    const QWidget *w1 = taskDeclPanel->getTaskPointWidget(legIndex);
    const QWidget *w2 = taskDeclPanel->getTaskPointWidget(legIndex+1);

    if (w1 == NULL || w2 == NULL) {
      break;
    }

    // Finds centers of taskpoint widgets
    int x1 = w1->pos().x() + w1->rect().center().x();
    int x2 = w2->pos().x() + w2->rect().center().x();

    // Gets distance and azimuth of the leg.
    qreal dist = 0.0, azimuth = 0.0;
    dist = taskData->distanceAzimuth(legIndex, legIndex+1, &azimuth);

    QString text = QString("%1 km / %2%3")
      .arg(0.001*dist, 0, 'f', 1).arg(azimuth, 0, 'f', 0)
      .arg(QChar(0xB0));

    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draws text with distance and azimuth
    QRect rect;
    rect.setRect(x1, 0, (x2-x1)+1, height());
    painter.drawText(rect, Qt::AlignCenter, text);

    painter.setRenderHint(QPainter::Antialiasing, false);

    // Counts text width
    int textWidth = fontMetrics.width(text);

    // Draws line between task points.
    int ofs = 2;  // margin of line ends.
    int scale = 6;  // length of arrow head
    painter.drawLine(x1 + ofs + scale / 2, y,
      (x1 + x2 - textWidth) / 2 - ofs, y);
    painter.drawLine((x1 + x2 + textWidth) / 2 + ofs, y,
      x2 - ofs - scale / 2, y);

    // Draws vertical lines between legs.
    if (legIndex == 0) {
      painter.drawLine(x1, y - scale, x1, height()-1);
    }
    painter.drawLine(x2, y - scale, x2, height()-1);

    // Draws horizontal lines at the begin and the end of task axis.
    if (legIndex == 0) {
      painter.drawLine(0, y, x1, y);
    }
    if (legIndex+2 == taskData->size()) {
      painter.drawLine(x2, y, width()-1, y);
    }

    painter.setRenderHint(QPainter::Antialiasing, true);

    // Draws 1st arrow head
    QPointF arrowHeadPts[3];
    arrowHeadPts[0] = QPointF(x1 + ofs + 1, y + 0.5);
    arrowHeadPts[1] = QPointF(x1 + scale + ofs + 1, y - scale / 2 + 0.5);
    arrowHeadPts[2] = QPointF(x1 + scale + ofs + 1, y + scale / 2 + 0.5);
    painter.drawPolygon(arrowHeadPts, 3);

    // Draws 2nd arrow head
    arrowHeadPts[0] = QPointF(x2 - ofs , y + 0.5);
    arrowHeadPts[1] = QPointF(x2 - scale - ofs, y - scale / 2 + 0.5);
    arrowHeadPts[2] = QPointF(x2 - scale - ofs, y + scale / 2 + 0.5);
    painter.drawPolygon(arrowHeadPts, 3);
  }

  taskFile->endRead();
}

}  // End namespace Updraft
