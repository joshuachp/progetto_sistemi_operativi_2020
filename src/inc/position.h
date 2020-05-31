/// @file position.h
/// @brief Contiene la definizione delle funzioni e strutture per la posizione
///        sulla scacchiera

#include <stdint.h>
#include <stdlib.h>

#pragma once

/*
 * Struct for the tuple of position on the board
 */
typedef struct vec_2 {
  uint8_t i;
  uint8_t j;
} vec_2;

/*
 * Node fore double linked list for positions
 */
typedef struct node_positions {
  vec_2 value[5];
  struct node_positions *next;
  struct node_positions *prev;
} node_positions;

/*
 * Node fore double linked list for positions
 */
typedef struct list_positions {
  struct node_positions *head;
  struct node_positions *tail;
  size_t length;
} list_positions;

/*
 * Square value of the Euclid distance of two points on the board
 *
 * @param arg1 Point a
 * @param arg2 Point b
 * @return Distance from a to b
 */
uint8_t distance_sqr(vec_2 arg1, vec_2 arg2);

/*
 * Allocate ad initialize a list of positions
 * XXX: Adding more control over the list probably never using it
 *
 * @param head The head of the list
 * @param tail The tail of the list
 * @param length The length of the list
 * @return Pointer to the allocated list
 */
list_positions *create_list_positions(node_positions *head,
                                      node_positions *tail, size_t length);

/*
 * Free a list of positions, it will free each node and value
 *
 * @param list List to free
 */
void free_list_positions(list_positions *list);

/**
 * Appends the node to list, double linking it. Node should have next and prev
 * set to NULL.
 *
 * @param list The list
 * @param node Node to append
 */
void append_list_positions(list_positions *list, node_positions *node);
