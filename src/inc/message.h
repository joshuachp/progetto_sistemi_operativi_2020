/// @file message.h
/// @brief  Contains the definition for the functions and structures for the
///         messages.

#pragma once

#include <stdint.h>
#include <sys/types.h>

/**
 * Messages sent from the clients to the devices
 */
typedef struct {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  char message[256];
  // NOTE: Changed from double because we use the squared values to check the
  // distances, so the max_distance (squared) max value can only be
  // 10^2+10^2=200, that is approximately 15 squared.
  uint8_t max_distance;
} Message;

/*
 * Node fore double linked list for message
 */
typedef struct node_message {
  Message value;
  struct node_message *next;
  struct node_message *prev;
} node_message;

/*
 * Node fore double linked list for message
 */
typedef struct list_message {
  struct node_message *head;
  struct node_message *tail;
  size_t length;
} list_message;

/*
 * Allocate ad initialize a list of message
 * XXX: Adding more control over the list probably never using it
 *
 * @param head The head of the list
 * @param tail The tail of the list
 * @param length The length of the list
 * @return Pointer to the allocated list
 */
list_message *create_list_message(node_message *head, node_message *tail,
                                  size_t length);

/*
 * Free a list of message, it will free each node and values
 *
 * @param list List to free
 */
void free_list_message(list_message *list);

/**
 * Appends the node to list, double linking it. Node should have next and prev
 * set to NULL.
 *
 * @param list The list
 * @param node Node to append
 */
void append_list_message(list_message *list, node_message *node);
