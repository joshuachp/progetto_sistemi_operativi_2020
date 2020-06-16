/// @file fifo.cpp
/// @brief Contiene i tests delle funzioni del client

#include "gtest/gtest.h"

extern "C" {
#include "client_lib.h"
}

TEST(write_out_message_id, ok) {
  Message message = {0, 1, 2, "Test", 200};
  time_t t = time(NULL);
  Acknowledgment ack[5] = {
      {0, 1, 2, t}, {1, 2, 2, t}, {2, 3, 2, t}, {3, 4, 2, t}, {4, 5, 2, t}};
  ASSERT_NO_FATAL_FAILURE(write_out_message_id(&message, ack));
}
