/// @file defines.cpp
/// @brief  Contains the tests for the functions and structures for the
///         messages and acknowledgment.

#include "gtest/gtest.h"

extern "C" {
#include "defines.h"
}

/*
 * Test creation of message list
 */
TEST(create_list_message, base) {
  list_message *list = create_list_message(NULL, NULL, 0);
  ASSERT_TRUE(list->head == NULL);
  ASSERT_TRUE(list->tail == NULL);
  ASSERT_EQ(0, list->length);
}

/*
 * Test creation of message list
 */
TEST(create_list_message, value) {
  node_message node = {{0, 0, 1, "Test", 200}, NULL, NULL};
  list_message *list = create_list_message(&node, &node, 1);
  ASSERT_TRUE(list->head != NULL);
  ASSERT_TRUE(list->head->next == NULL);
  ASSERT_TRUE(list->head->prev == NULL);
  ASSERT_EQ(list->length, 1);
}

/*
 * Test creation of message list
 */
TEST(free_list_message, base) {
  node_message node_stack = {{0, 0, 1, "Test", 200}, NULL, NULL};
  node_message *node = (node_message *)malloc(sizeof(node_message));
  memcpy(node, &node_stack, sizeof(node_message));
  list_message *list = create_list_message(node, node, 1);
  ASSERT_TRUE(list->head != NULL);
  ASSERT_NO_FATAL_FAILURE(free_list_message(list));
}

/*
 * Test append of message list
 */
TEST(append_list_message, ok) {
  node_message node_stack = {{0, 0, 1, "Test", 200}, NULL, NULL};
  node_message *node1 = (node_message *)malloc(sizeof(node_message));
  node_message *node2 = (node_message *)malloc(sizeof(node_message));
  memcpy(node1, &node_stack, sizeof(node_message));
  memcpy(node2, &node_stack, sizeof(node_message));
  list_message *list = create_list_message(node1, node1, 1);
  append_list_message(list, node2);
  ASSERT_EQ(2, list->length);
  ASSERT_EQ(list->tail, node2);
  ASSERT_EQ(list->head->next, node2);
  free_list_message(list);
}

/*
 * Test append of message list
 */
TEST(append_list_message, null_list) {
  node_message node_stack = {{0, 0, 1, "Test", 200}, NULL, NULL};
  node_message *node = (node_message *)malloc(sizeof(node_message));
  memcpy(node, &node_stack, sizeof(node_message));
  list_message *list = create_list_message(NULL, NULL, 0);
  append_list_message(list, node);
  ASSERT_EQ(1, list->length);
  ASSERT_EQ(list->tail, node);
  ASSERT_EQ(list->head, node);
  free_list_message(list);
}

TEST(remove_list_message, base) {
  node_message node_stack = {{0, 0, 1, "Test", 200}, NULL, NULL};
  node_message *node = (node_message *)malloc(sizeof(node_message));
  memcpy(node, &node_stack, sizeof(node_message));
  list_message *list = create_list_message(node, node, 1);
  ASSERT_EQ(1, list->length);
  remove_list_message(list, node);
  ASSERT_EQ(0, list->length);
  ASSERT_EQ(NULL, list->head);
  ASSERT_EQ(NULL, list->tail);
  free_list_message(list);
}

TEST(remove_list_message, multiple) {
  node_message node_stack = {{0, 0, 1, "Test", 200}, NULL, NULL};
  node_message *node1 = (node_message *)malloc(sizeof(node_message));
  node_message *node2 = (node_message *)malloc(sizeof(node_message));
  memcpy(node1, &node_stack, sizeof(node_message));
  memcpy(node2, &node_stack, sizeof(node_message));
  list_message *list = create_list_message(node1, node1, 1);
  append_list_message(list, node2);
  ASSERT_EQ(2, list->length);
  remove_list_message(list, node1);
  ASSERT_EQ(1, list->length);
  ASSERT_EQ(node2, list->head);
  ASSERT_EQ(node2, list->tail);
  remove_list_message(list, node2);
  ASSERT_EQ(0, list->length);
  ASSERT_EQ(NULL, list->head);
  ASSERT_EQ(NULL, list->tail);
  free_list_message(list);
}
