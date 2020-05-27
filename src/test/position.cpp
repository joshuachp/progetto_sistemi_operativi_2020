/// @file position.c
/// @brief Contiene i tests delle funzioni specifiche per la posizione
/// sulla scacchiera

#include "gtest/gtest.h"

extern "C" {
#include "position.h"
}

/**
 * Test on a null distance
 */
TEST(distance_sqr, null) { ASSERT_EQ(distance_sqr({0, 0}, {0, 0}), 0); }

/*
 * Test on a casual distance
 */
TEST(distance_sqr, ok) { ASSERT_EQ(distance_sqr({1, 2}, {3, 4}), 8); }

/*
 * Test on the max distance
 */
TEST(distance_sqr, max) { ASSERT_EQ(distance_sqr({0, 0}, {10, 10}), 200); }
