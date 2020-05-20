/// @file position.c
/// @brief Contiene i tests delle funzioni specifiche per la posizione
/// sulla scacchiera

#include "gtest/gtest.h"

extern "C" {
#include "position.h"
}

TEST(distance_sqr, null) { ASSERT_EQ(distance_sqr({0, 0}, {0, 0}), 0); }
TEST(distance_sqr, ok) { ASSERT_EQ(distance_sqr({1, 2}, {3, 4}), 8); }
TEST(distance_sqr, max) { ASSERT_EQ(distance_sqr({0, 0}, {10, 10}), 200); }
