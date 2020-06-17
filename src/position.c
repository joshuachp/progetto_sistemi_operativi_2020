/// @file position.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la posizione
/// sulla scacchiera

#include "position.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t distance_sqr(vec_2 a, vec_2 b) {
  // Difference of the two
  int8_t j = a.j - b.j;
  int8_t i = a.i - b.i;
  // Return the squared sum
  return i * i + j * j;
}

list_positions *create_list_positions(node_positions *head,
                                      node_positions *tail, size_t length) {
  list_positions *list = malloc(sizeof(list_positions));
  list->head = head;
  list->tail = tail;
  list->length = length;
  return list;
}

void free_list_positions(list_positions *list) {
  node_positions *current = list->head;
  node_positions *next = NULL;
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

void append_list_positions(list_positions *list, node_positions *node) {
  if (list->tail != NULL) {
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
    list->length++;
  } else {
    list->head = node;
    list->tail = node;
    list->length = 1;
    node->prev = NULL;
  }
  node->next = NULL;
}
