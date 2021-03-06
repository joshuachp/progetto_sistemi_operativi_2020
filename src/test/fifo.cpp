/// @file fifo.cpp
/// @brief Contiene i tests delle funzioni sulle FIFO

#include "gtest/gtest.h"

extern "C" {
#include "fifo.h"
}

/**
 * Test for the path of a device from the PID
 */
TEST(pid_fifo_path, ok) {
  char *path = pid_fifo_path(1234);
  ASSERT_TRUE(strcmp("/tmp/dev_fifo.1234", path) == 0);
}
