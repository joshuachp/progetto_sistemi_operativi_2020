/// @file fifo.cpp
/// @brief Contiene i tests delle funzioni del client

#include "gtest/gtest.h"

extern "C" {
#include "client_lib.h"
}

// Test for distance inside
TEST(max_distance_sqr, less_equal) { ASSERT_LE(200, max_distance_sqr(15)); }

// Test for distance not inside
TEST(max_distance_sqr, greater) { ASSERT_GT(200, max_distance_sqr(14)); }

// Test for distance root of 200(10^2+10^2)
TEST(max_distance_sqr, less_equal_root_200) {
  // It's actually equal
  ASSERT_LE(200, max_distance_sqr(14.142135624));
}

// Test for distance root of 181(10^2+9^2)
TEST(max_distance_sqr, less_equal_root_181) {
  // It's actually equal
  ASSERT_EQ(181, max_distance_sqr(13.453624047));
}

TEST(write_out_message_id, ok) {
  Message message = {0, 1, 2, "Test", 200};
  time_t t = time(NULL);
  Acknowledgment ack[5] = {
      {0, 1, 2, t}, {1, 2, 2, t}, {2, 3, 2, t}, {3, 4, 2, t}, {4, 5, 2, t}};
  ASSERT_NO_FATAL_FAILURE(write_out_message_id(&message, ack));
}
