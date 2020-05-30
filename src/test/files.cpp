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
  list_positions *positions = read_positions_file(filename);
  ASSERT_TRUE(positions != NULL);
  ASSERT_EQ(5, positions->length);
  node_positions *node = positions->head;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ASSERT_EQ(expected[i][j].i, node->value[j].i);
      ASSERT_EQ(expected[i][j].j, node->value[j].j);
    }
    node = node->next;
  }
}

/**
 * Test on the position with big input file
 */
TEST(read_positions_file, input_big) {
  char filename[] = "../input/big_file_posizioni.txt";
  list_positions *positions = read_positions_file(filename);
  ASSERT_TRUE(positions != NULL);
  ASSERT_EQ(526, positions->length);
}

/**
 * Test string to position array
 */
TEST(parse_position_str, ok) {
  vec_2 expected[5] = {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 2}};
  char str[] = "0,0|1,0|2,0|0,1|1,2";
  node_positions *positions = parse_position_str(str);
  ASSERT_TRUE(positions != NULL);
  ASSERT_TRUE(positions->next == NULL);
  ASSERT_TRUE(positions->prev == NULL);
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(expected[i].i, positions->value[i].i);
    ASSERT_EQ(expected[i].j, positions->value[i].j);
  }
  free(positions);
}

/**
 * Test malformed string
 */
TEST(parse_position_str, malformed) {
  char str[] = "0,0|1,0 20|0,11,";
  node_positions *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test malformed string length ok null position
 */
TEST(parse_position_str, malformed_lenght_ok_null_position) {
  char str[] = "0,0|1,0|2,0||1,1|1,2";
  node_positions *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test malformed string length ok
 */
TEST(parse_position_str, malformed_length_ok_4_positions) {
  char str[] = "|0,0|1,0|2,0|||||1,2";
  node_positions *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test malformed string length ok multi commas
 */
TEST(parse_position_str, malformed_length_ok_commas) {
  char str[] = "0,0,1,0|2,0|0,1|1,2";
  node_positions *positions = parse_position_str(str);
  ASSERT_TRUE(positions == NULL);
}

/**
 * Test null string
 */
TEST(parse_position_str, null) {
  node_positions *positions = parse_position_str(NULL);
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
  for (int i = 0; i < 3; i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(line != NULL);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
    free(line);
    line = NULL;
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
  for (int i = 0; i < 3; i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(line != NULL);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
    free(line);
    line = NULL;
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
  for (int i = 0; i < 4; i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
    free(line);
    line = NULL;
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
  for (int i = 0; i < 5; i++) {
    line = get_next_line_buf(buf, start);
    ASSERT_TRUE(expected[i].compare(std::string(line)) == 0);
    start += strlen(line) + 1;
    free(line);
    line = NULL;
  }
}
