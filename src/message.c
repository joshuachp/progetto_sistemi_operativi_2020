/// @file message.c
/// @brief  Contains the implementation for the functions and structures for
///         the messages.

#include "message.h"
#include <stdlib.h>

list_message *create_list_message(node_message *head, node_message *tail,
                                  size_t length) {
  list_message *list = malloc(sizeof(list_message));
  list->head = head;
  list->tail = tail;
  list->length = length;
  return list;
}

void free_list_message(list_message *list) {
  node_message *current = list->head;
  node_message *next = NULL;
  while (current != NULL) {
    next = current->next;
    // Free the all struct because value is an array
    free(current);
    current = next;
    next = NULL;
  }
  free(list);
  list = NULL;
}

void append_list_message(list_message *list, node_message *node) {
  if (list->tail != NULL) {
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
    list->length++;
  } else {
    list->head = node;
    list->tail = node;
    list->length = 1;
  }
}
