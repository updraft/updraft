#include "plot.h"

#include <QPoint>
#include <QPen>

namespace Updraft {
namespace IgcViewer {



void PlotWidget::paintEvent(QPaintEvent* paintEvent) {
  QPen pen;
  QColor color;

  QPen axisPen(axisColor);

  int lengthX = width() - 2*pixelOffsetX;
  int lengthY = height() - 2*pixelOffsetY;

  QPoint zero;
  QPoint maximum;

  // fill background

  // draw the altitude graph
  // draw axes
  zero.setX(pixelOffsetX);
  zero.setY(pixelOffsetY + 0.25*lengthY);
  maximum.setX(pixelOffsetX + lengthX);
  maximum.setY(pixelOffsetY);

  // for (int i=1; i<=altitudeInfo->fixList->size(); )
}

}  // End namespace IgcViewer
}  // End namespace Updraft
