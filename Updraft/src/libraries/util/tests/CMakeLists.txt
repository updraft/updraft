cmake_minimum_required(VERSION 2.8)

TEST_BUILD(test_location ONLY_FILES location.cpp location.h)
TARGET_LINK_LIBRARIES(test_location util)
