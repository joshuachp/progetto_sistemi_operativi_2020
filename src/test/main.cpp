/// @file position.c
/// @brief Contiene l'implementazione per l'avvio dei tests

#include "gtest/gtest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
