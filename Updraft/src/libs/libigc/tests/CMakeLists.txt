FIND_PACKAGE(Qt4 REQUIRED)


QT4_WRAP_CPP(HEADERS_MOC testlibigc.h)
ADD_UNIT_TEST(test_libigc testlibigc.cpp ${HEADERS_MOC})

TARGET_LINK_LIBRARIES(test_libigc libigc)
