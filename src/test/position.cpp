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
  list_positions *list = create_list_positions(NULL, NULL);
  ASSERT_TRUE(list->head == NULL);
  ASSERT_TRUE(list->tail == NULL);
}

/*
 * Test creation of positions list
 */
TEST(create_list_positions, value) {
  node_position node = {.value = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}},
                        .next = NULL,
                        .prev = NULL};
  list_positions *list = create_list_positions(&node, &node);
  ASSERT_TRUE(list->head != NULL);
  ASSERT_TRUE(list->head->next == NULL);
  ASSERT_TRUE(list->head->prev == NULL);
  ASSERT_EQ(list->head->value[0].i, 0);
  ASSERT_EQ(list->head->value[0].j, 1);
}
/*
 * Test creation of positions list
 */
TEST(free_list_positions, base) {
  node_position node_stack = {.value = {{0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}},
                              .next = NULL,
                              .prev = NULL};
  node_position *node = (node_position *)malloc(sizeof(node_position));
  std::memcpy(node, &node_stack, sizeof(node_position));
  list_positions *list = create_list_positions(node, node);
  ASSERT_TRUE(list->head != NULL);
  ASSERT_NO_THROW(free_list_positions(list));
}
