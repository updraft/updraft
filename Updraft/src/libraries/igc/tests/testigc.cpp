#include "testigc.h"

#include <math.h>

#include <QtTest>

namespace Updraft {
namespace Igc {
namespace Test {

/// Loads a test file before running all the other tests.
void TestIgc::initTestCase() {
  loadTestFile();
}

/// Reads a manually crafted IGC file and verifies that values from
/// H records are as expected.
void TestIgc::testHRecords() {
  QCOMPARE(igc.altimeterSetting(), 1.0);
  QCOMPARE(igc.competitionClass(), QString("2"));
  QCOMPARE(igc.competitionId(), QString("3"));
  QCOMPARE(igc.manufacturer(), QString("4"));
  QCOMPARE(igc.frType(), QString("5"));
  QCOMPARE(igc.gliderId(), QString("6"));
  QCOMPARE(igc.gps(), QString("7"));
  QCOMPARE(igc.gliderType(), QString("11"));
  QCOMPARE(igc.pilot(), QString("12"));
}

/// Reads a manually crafted IGC file and tests
/// B records are as expected.
void TestIgc::testBRecords() {
  int count = 15;

  int value = 1;
  QTime time0(0, 0, 0, 0);

  Igc::EventMapIterator iterator(igc.events());

  for(int i = 0; i < count; ++i) {
    QVERIFY(iterator.hasNext());

    QTime time = time0.addSecs(abs(value));
    double lat = value / 60000.0;
    double lon = value / 60000.0;

    bool valid = value > 0;
    
    double latDeg, latMin, latMinDec;
    latMin = modf(fabs(lat), &latDeg) * 60;
    latMinDec = modf(latMin, &latMin) * 1000;
    char latSign = lat > 0 ? 'N' : 'S';

    double lonDeg, lonMin, lonMinDec;
    lonMin = modf(fabs(lon), &lonDeg) * 60;
    lonMinDec = modf(lonMin, &lonMin) * 1000;
    char lonSign = lon > 0 ? 'E' : 'W';

    double pressureAlt = value;
    double gpsAlt = abs(value);

    qDebug("Expect: B%02d%02d%02d%02d%02d%03d%c%03d%02d%03d%c%c%05d%05d",
      time.hour(), time.minute(), time.second(),
      static_cast<int>(latDeg), static_cast<int>(latMin),
      static_cast<int>(latMinDec), latSign,
      static_cast<int>(lonDeg), static_cast<int>(lonMin),
      static_cast<int>(lonMinDec), lonSign,
      (valid ? 'A' : 'V'),
      static_cast<int>(pressureAlt),
      static_cast<int>(gpsAlt));
    
    iterator.next();
    Event* ev = iterator.value();
  
    QCOMPARE(ev->type, Event::FIX);

    Fix* fix = static_cast<Fix*>(ev);

    QCOMPARE(fix->timestamp, time);
    QCOMPARE(fix->lat, lat);
    QCOMPARE(fix->lon, lon);
    QCOMPARE(fix->valid, valid);
    QCOMPARE(fix->pressureAlt, pressureAlt);
    QCOMPARE(fix->gpsAlt, gpsAlt);

    value *= -2;
  }
  QVERIFY(!iterator.hasNext());
}


/// Test that the clean method really deletes all information
/// and empties the eventMap.
/// Loads the test file again after checking everything.
void TestIgc::testClean() {
  igc.clear();

  QCOMPARE(igc.altimeterSetting(), 0.0);
  QCOMPARE(igc.competitionClass(), QString());
  QCOMPARE(igc.competitionId(), QString());
  QCOMPARE(igc.manufacturer(), QString());
  QCOMPARE(igc.frType(), QString());
  QCOMPARE(igc.gliderId(), QString());
  QCOMPARE(igc.gps(), QString());
  QCOMPARE(igc.gliderType(), QString());
  QCOMPARE(igc.pilot(), QString());

  Igc::EventMapIterator iterator(igc.events());
  QVERIFY(!iterator.hasNext());

  loadTestFile();
}

/// Loads a test file.
void TestIgc::loadTestFile() {
  QVERIFY(igc.load(TEST_DATA_DIR "/testigc.igc"));
}

}  // End namespace Test
}  // End namespace Igc
}  // End namespace Updraft

QTEST_MAIN(Updraft::Igc::Test::TestIgc)
