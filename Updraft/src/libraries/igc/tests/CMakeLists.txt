QT4_WRAP_CPP(HEADERS_MOC testigc.h)
ADD_UNIT_TEST(test_igc testigc.cpp ${HEADERS_MOC})

STYLE_CHECK(testigc.h testigc.cpp)

TARGET_LINK_LIBRARIES(test_igc igc)
