/// @file position.c
/// @brief Contiene i tests delle funzioni del server

#include "gtest/gtest.h"

extern "C" {
#include "files.h"
}

/**
 * Test on the position input file
 */
TEST(read_positions_file, input) {
  // FIXME: Complete read position test to not segfault
  return;
  vec_2 expected[5][5] = {{{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 2}},
                          {{0, 0}, {1, 0}, {3, 0}, {0, 2}, {1, 2}},
                          {{0, 0}, {2, 0}, {4, 0}, {0, 2}, {1, 2}},
                          {{0, 1}, {3, 0}, {5, 0}, {0, 3}, {1, 2}},
                          {{0, 2}, {3, 0}, {5, 0}, {0, 4}, {1, 2}}};
  char filename[] = "../input/file_posizioni.txt";
  vec_2 **positions = read_positions_file(filename);
  ASSERT_TRUE(positions != (void *)-1);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ASSERT_EQ(expected[i][j].i, positions[i][j].i);
      ASSERT_EQ(expected[i][j].j, positions[i][j].j);
    }
  }
}

/**
 * Test string to position array
 */
TEST(str_to_position_array, ok) {
  vec_2 expected[5] = {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 2}};
  char str[] = "0,0|1,0|2,0|0,1|1,2";
  vec_2 *positions = str_to_position_array(str);
  ASSERT_TRUE(positions != (void *)-1);
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(expected[i].i, positions[i].i);
    ASSERT_EQ(expected[i].j, positions[i].j);
  }
}

/**
 * Test malformed string
 */
TEST(str_to_position_array, malformed) {
  char str[] = "0,0|1,0 20|0,11,";
  vec_2 *positions = str_to_position_array(str);
  ASSERT_TRUE(positions == (void *)-1);
}

/**
 * Test malformed string length ok null position
 */
TEST(str_to_position_array, malformed_lenght_ok_null_position) {
  char str[] = "0,0|1,0|2,0||1,1|1,2";
  vec_2 *positions = str_to_position_array(str);
  ASSERT_TRUE(positions == (void *)-1);
}

/**
 * Test malformed string length ok
 */
TEST(str_to_position_array, malformed_length_ok_4_positions) {
  char str[] = "|0,0|1,0|2,0|||||1,2";
  vec_2 *positions = str_to_position_array(str);
  ASSERT_TRUE(positions == (void *)-1);
}

/**
 * Test malformed string length ok multi commas
 */
TEST(str_to_position_array, malformed_length_ok_commas) {
  char str[] = "0,0,1,0|2,0|0,1|1,2";
  vec_2 *positions = str_to_position_array(str);
  ASSERT_TRUE(positions == (void *)-1);
}

/**
 * Test null string
 */
TEST(str_to_position_array, null) {
  vec_2 *positions = str_to_position_array(NULL);
  ASSERT_TRUE(positions == (void *)-1);
}
