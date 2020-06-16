/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define BUF_READ_SIZE 256
#define MIN_POSITIONS_ARRAY_LENGTH 6
#define BOARD_SIZE 10
#define DEVICE_NUMBER 5
#define ACK_SIZE (DEVICE_NUMBER * 20)
#define SLEEP_TIME_SERVER 2

/**
 * Messages sent from the clients to the devices
 */
typedef struct {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  char message[256];
  // Squared value of the max_distance for confront without using the square
  // root
  double max_distance;
} Message;

/**
 * Acknowledgement saved in the shared memory
 */
typedef struct Acknowledgement {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  time_t timestamp;
} Acknowledgment;

/**
 * Acknowledgement sent to the client
 */
typedef struct ack_msg {
  long mtype;
  Acknowledgment ack_list[DEVICE_NUMBER];
} ack_msg;

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

/**
 * Send a message to a device throw its FIFO.
 *
 * @param  message The message to send
 */
void send_message_device(Message *message);
