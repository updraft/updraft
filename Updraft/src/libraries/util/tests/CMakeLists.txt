QT4_WRAP_CPP(HEADERS_MOC testlocation.h)
ADD_UNIT_TEST(test_location testlocation.cpp ${HEADERS_MOC})

STYLE_CHECK(testlocation.h testlocation.cpp)

TARGET_LINK_LIBRARIES(test_location util)
