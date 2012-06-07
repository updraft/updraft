#include "location.h"

namespace Updraft {
namespace Util {

Location::Location()
  : lat(0.0), lon(0.0), alt(0.0) {
}

void Location::latFromRadians(qreal radians) {
  lat = radians / M_PI * 180;
}

void Location::lonFromRadians(qreal radians) {
  lon = radians / M_PI * 180;
}

void Location::latFromDMS(qreal d, qreal m, qreal s, char sign) {
  lat = d + (m + s / 60.0) / 60.0;

  if (sign == 'S') {
    lat = -lat;
  }
}

void Location::lonFromDMS(qreal d, qreal m, qreal s, char sign) {
  lon = d + (m + s / 60.0) / 60.0;

  if (sign == 'W') {
    lon = -lon;
  }
}

void Location::latToDMS(int *d, int *m, qreal *s, char *sign) const {
  *sign = lat < 0 ? 'S' : 'N';

  qreal latAbs = qAbs(lat);
  *d = qFloor(latAbs);
  *m = qFloor((latAbs - (qreal)*d)*60.0);
  *s = ((latAbs - (qreal)*d)*60.0 - (qreal)*m)*60.0;
}

void Location::lonToDMS(int *d, int *m, qreal *s, char *sign) const {
  *sign = lon < 0 ? 'W' : 'E';

  qreal lonAbs = qAbs(lon);
  *d = qFloor(lonAbs);
  *m = qFloor((lonAbs - (qreal)*d)*60.0);
  *s = ((lonAbs - (qreal)*d)*60.0 - (qreal)*m)*60.0;
}

QString Location::dmsToString(int d, int m, qreal s, char sign) {
  return QString("%1%2 %3%4 %5%6 %7").
    arg(d).arg(QChar(0xB0)).
    arg(m).arg(QChar(0x2032)).
    arg(s, 0, 'g', 2).arg(QChar(0x2033)).
    arg(sign);
}

QString Location::latToString() const {
  int d, m;
  qreal s;
  char sign;
  latToDMS(&d, &m, &s, &sign);
  return Location::dmsToString(d, m, s, sign);
}

QString Location::lonToString() const {
  int d, m;
  qreal s;
  char sign;
  lonToDMS(&d, &m, &s, &sign);
  return Location::dmsToString(d, m, s, sign);
}

}  // End namespace Util
}  // End namespace Updraft
