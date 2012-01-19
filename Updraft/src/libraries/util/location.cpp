#include "location.h"

namespace Updraft {
namespace Util {

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

}  // End namespace Util
}  // End namespace Updraft

