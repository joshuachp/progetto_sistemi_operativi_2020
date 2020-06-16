/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

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

void send_message_device(Message *message) {
  // Open FIFO device
  char *path = pid_fifo_path(message->pid_receiver);
  int fd = open(path, O_WRONLY);
  if (fd == -1)
    err_exit("open", __FILE__, __LINE__);

  // Write message
  if (write(fd, message, sizeof(Message)) < (ssize_t)sizeof(Message))
    err_exit("write", __FILE__, __LINE__);

  // Close FIFO device
  if (close(fd) == -1)
    err_exit("close", __FILE__, __LINE__);

  // Free
  free(path);
}
