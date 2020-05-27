/// @file files.cpp
/// @brief Contiene i tests delle funzioni sui files

#include "gtest/gtest.h"

extern "C" {
#include "files.h"
}

/**
 * Test on the position input file
 */
TEST(read_positions_file, input) {
  vec_2 expected[5][5] = {{{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 2}},
                          {{0, 0}, {1, 0}, {3, 0}, {0, 2}, {1, 2}},
                          {{0, 0}, {2, 0}, {4, 0}, {0, 2}, {1, 2}},
                          {{0, 1}, {3, 0}, {5, 0}, {0, 3}, {1, 2}},
                          {{0, 2}, {3, 0}, {5, 0}, {0, 4}, {1, 2}}};
  char filename[] = "../input/file_posizioni.txt";
  vec_2 **positions = read_positions_file(filename);
  ASSERT_TRUE(positions != NULL);
  size_t p_length = 0;
  while (positions[p_length] != NULL) {
    p_length++;
  }
  ASSERT_EQ(5, p_length);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ASSERT_EQ(expected[i][j].i, positions[i][j].i);
      ASSERT_EQ(expected[i][j].j, positions[i][j].j);
    }
  }
}

//
// /**
//  * Test on the position with big input file
//  */
// TEST(read_positions_file, input_big) {
//   char filename[] = "../input/big_file_posizioni.txt";
//   vec_2 **positions = read_positions_file(filename);
//   ASSERT_TRUE(positions != NULL);
//   size_t p_length = 0;
//   while (positions[p_length] != NULL) {
//     p_length++;
//   }
//   ASSERT_EQ(5, p_length);
// }

/**
 * Test string to position array
 */
TEST(parse_position_str, ok) {
  vec_2 expected[5] = {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 2}};
  char str[] = "0,0|1,0|2,0|0,1|1,2";
  vec_2 *positions = parse_position_str(str);
  ASSERT_TRUE(positions != NULL);
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(expected[i].i, positions[i].i);
    ASSERT_EQ(expected[i].j, positions[i].j);
  }
}

/**
 * Test malformed string
 */
TEST(parse_position_str, malformed) {
  char str[] = "0,0|1,0 20|0,11,";
  vec_2 *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test malformed string length ok null position
 */
TEST(parse_position_str, malformed_lenght_ok_null_position) {
  char str[] = "0,0|1,0|2,0||1,1|1,2";
  vec_2 *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test malformed string length ok
 */
TEST(parse_position_str, malformed_length_ok_4_positions) {
  char str[] = "|0,0|1,0|2,0|||||1,2";
  vec_2 *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test malformed string length ok multi commas
 */
TEST(parse_position_str, malformed_length_ok_commas) {
  char str[] = "0,0,1,0|2,0|0,1|1,2";
  vec_2 *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test null string
 */
TEST(parse_position_str, null) {
  vec_2 *positions = parse_position_str(NULL);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test get lines of buf
 */
TEST(get_next_line_buf, ok) {
  char buf[] = "Test multi\n"
               " lines \n"
               "strings";
  std::string expected[] = {"Test multi", " lines ", "strings"};
  size_t start = 0;
  char *line;
  for (int i = 0; i < expected->length(); i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(line != NULL);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
  }
}

/**
 * Test get lines of buf newline at the end
 */
TEST(get_next_line_buf, new_line_end) {
  char buf[] = "Test multi\n"
               " lines \n"
               "strings\n";
  std::string expected[] = {"Test multi", " lines ", "strings"};
  size_t start = 0;
  char *line;
  for (int i = 0; i < expected->length(); i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(line != NULL);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
  }
}

/**
 * Test get lines of buf double newline at the end
 */
TEST(get_next_line_buf, double_new_line_end) {
  char buf[] = "Test multi\n"
               " lines \n"
               "strings\n"
               "\n";
  std::string expected[] = {"Test multi", " lines ", "strings", ""};
  size_t start = 0;
  char *line;
  for (int i = 0; i < expected->length(); i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
  }
}

/**
 * Test get lines of buf double newline at the beginning
 */
TEST(get_next_line_buf, double_new_line_beginning) {
  char buf[] = "\n"
               "\n"
               "Test multi\n"
               " lines \n"
               "strings\n";
  std::string expected[] = {"", "", "Test multi", " lines ", "strings"};
  size_t start = 0;
  char *line;
  for (int i = 0; i < expected->length(); i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
  }
}
