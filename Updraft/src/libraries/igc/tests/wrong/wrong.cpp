#include "wrong.h"

#include <QtTest>

#include "igc.h"

namespace Updraft {
namespace Igc {
namespace Test {

void Wrong::a_record() {
  Igc::IgcFile f;
  QVERIFY(!f.load(TEST_DATA_DIR "/wrong_a_record.igc"));
}

void Wrong::b_record() {
  Igc::IgcFile f;
  QVERIFY(!f.load(TEST_DATA_DIR "/wrong_b_record.igc"));
}

}  // End namespace Test
}  // End namespace Igc
}  // End namespace Updraft

QTEST_MAIN(Updraft::Igc::Test::Wrong)
