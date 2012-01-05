#include "testlibigc.h"

#include <QtTest>

#include "igc.h"

namespace Updraft {
namespace Libigc {
namespace Test {

/// Reads a manually crafted IGC file and verifies that values from
/// H records are as expected.
void TestLibigc::testHRecords() {
  Updraft::Libigc::Igc igc;
  QVERIFY(igc.load(TEST_DATA_DIR "/h_records.igc"));

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

}  // End namespace Test
}  // End namespace Libigc
}  // End namespace Updraft

QTEST_MAIN(Updraft::Libigc::Test::TestLibigc)
