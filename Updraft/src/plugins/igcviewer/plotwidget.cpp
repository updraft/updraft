#include "plotwidget.h"

#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>

#include "plotpainters.h"

namespace Updraft {
namespace IgcViewer {

const QColor PlotWidget::BG_COLOR = Qt::black;
const QPen PlotWidget::ALTITUDE_PEN = QPen(Qt::red);
const QPen PlotWidget::VERTICAL_SPEED_PEN = QPen(Qt::blue);
const QPen PlotWidget::GROUND_SPEED_PEN= QPen(Qt::yellow);

static const qreal LN10 = qLn(10);

PlotWidget::PlotWidget(IgcInfo* altitudeInfo, IgcInfo* verticalSpeedInfo,
  IgcInfo *groundSpeedInfo)
  : altitudeInfo(altitudeInfo), verticalSpeedInfo(verticalSpeedInfo),
    groundSpeedInfo(groundSpeedInfo) {
  QVBoxLayout* layout = new QVBoxLayout();

  setLayout(layout);

  qreal maxTime = altitudeInfo->maxTime();

  altitudeAxes.setLimits(
    altitudeInfo->min(), altitudeInfo->max(), maxTime);
  layout->addItem(&altitudeAxes);

  verticalSpeedAxes.setLimits(
    verticalSpeedInfo->min(), verticalSpeedInfo->max(), maxTime);
  layout->addItem(&verticalSpeedAxes);

  groundSpeedAxes.setLimits(
    groundSpeedInfo->min(), groundSpeedInfo->max(), maxTime);
  layout->addItem(&groundSpeedAxes);

  layout->setStretch(0, 2);
  layout->setStretch(1, 1);
  layout->setStretch(2, 1);
}

void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), BG_COLOR);

  AltitudePlotPainter altitudePainter;
  altitudePainter.init(&painter, &altitudeAxes, altitudeInfo);
  altitudePainter.draw();

  VerticalSpeedPlotPainter verticalSpeedPainter;
  verticalSpeedPainter.init(&painter, &verticalSpeedAxes, verticalSpeedInfo);
  verticalSpeedPainter.draw();

  GroundSpeedPlotPainter groundSpeedPainter;
  groundSpeedPainter.init(&painter, &groundSpeedAxes, groundSpeedInfo);
  groundSpeedPainter.draw();
}

}  // End namespace IgcViewer
}  // End namespace Updraft
