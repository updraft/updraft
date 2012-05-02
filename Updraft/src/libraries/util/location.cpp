#include "location.h"

namespace Updraft {
namespace Util {

Location::Location()
  : lat(0.0), lon(0.0), alt(0.0) {
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

}  // End namespace Util
}  // End namespace Updraft
