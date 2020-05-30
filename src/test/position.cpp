/// @file position.c
/// @brief Contiene i tests delle funzioni specifiche per la posizione
/// sulla scacchiera

#include "gtest/gtest.h"
#include <cstring>

extern "C" {
#include "position.h"
}

/*
 * Test on a null distance
 */
TEST(distance_sqr, null) { ASSERT_EQ(distance_sqr({0, 0}, {0, 0}), 0); }

/*
 * Test on a casual distance
 */
TEST(distance_sqr, ok) { ASSERT_EQ(distance_sqr({1, 2}, {3, 4}), 8); }

/**
 * Test on the max distance
 */
TEST(distance_sqr, max) { ASSERT_EQ(distance_sqr({0, 0}, {10, 10}), 200); }

/*
 * Test creation of positions list
 */
TEST(create_list_positions, base) {
  list_positions *list = create_list_positions(NULL, NULL, 0);
  ASSERT_TRUE(list->head == NULL);
  ASSERT_TRUE(list->tail == NULL);
  ASSERT_EQ(0, list->length);
}

/*
 * Test creation of positions list
 */
TEST(create_list_positions, value) {
  node_positions node = {.value = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}},
                         .next = NULL,
                         .prev = NULL};
  list_positions *list = create_list_positions(&node, &node, 1);
  ASSERT_TRUE(list->head != NULL);
  ASSERT_TRUE(list->head->next == NULL);
  ASSERT_TRUE(list->head->prev == NULL);
  ASSERT_EQ(list->length, 1);
  ASSERT_EQ(list->head->value[0].i, 0);
  ASSERT_EQ(list->head->value[0].j, 1);
}

/*
 * Test creation of positions list
 */
TEST(free_list_positions, base) {
  node_positions node_stack = {
      .value = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}},
      .next = NULL,
      .prev = NULL};
  node_positions *node = (node_positions *)malloc(sizeof(node_positions));
  std::memcpy(node, &node_stack, sizeof(node_positions));
  list_positions *list = create_list_positions(node, node, 1);
  ASSERT_TRUE(list->head != NULL);
  ASSERT_NO_THROW(free_list_positions(list));
}

/*
 * Test append of positions list
 */
TEST(append_list_positions, ok) {
  node_positions node_stack = {
      .value = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}},
      .next = NULL,
      .prev = NULL};
  node_positions *node1 = (node_positions *)malloc(sizeof(node_positions));
  node_positions *node2 = (node_positions *)malloc(sizeof(node_positions));
  std::memcpy(node1, &node_stack, sizeof(node_positions));
  std::memcpy(node2, &node_stack, sizeof(node_positions));
  list_positions *list = create_list_positions(node1, node1, 1);
  append_list_positions(list, node2);
  ASSERT_EQ(2, list->length);
  ASSERT_EQ(list->tail, node2);
  ASSERT_EQ(list->head->next, node2);
  free_list_positions(list);
}

/*
 * Test append of positions list
 */
TEST(append_list_positions, null_list) {
  node_positions node_stack = {
      .value = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}},
      .next = NULL,
      .prev = NULL};
  node_positions *node = (node_positions *)malloc(sizeof(node_positions));
  std::memcpy(node, &node_stack, sizeof(node_positions));
  list_positions *list = create_list_positions(NULL, NULL, 0);
  append_list_positions(list, node);
  ASSERT_EQ(1, list->length);
  ASSERT_EQ(list->tail, node);
  ASSERT_EQ(list->head, node);
  free_list_positions(list);
}
