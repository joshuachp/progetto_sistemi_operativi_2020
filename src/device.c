/// @file device.c
/// @brief Contains the implementation of function and variables for the device

#include <stdio.h>

#include "defines.h"
#include "device.h"
#include "err_exit.h"
#include "fifo.h"
#include "semaphore.h"
#include "server_lib.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void device_process(uint8_t dev_num) {
  // Device pid
  pid_t pid = getpid();
  // Current device positin
  vec_2 position = {0, 0};
  // Message list
  list_message *messages = create_list_message(NULL, NULL, 0);

  // make FIFO
  make_fifo_device(pid);
  // open FIFO device
  char *path = pid_fifo_path(pid);
  int fifo = open(path, O_RDONLY | O_NONBLOCK);
  if (fifo == -1)
    err_exit("open", __FILE__, __LINE__);
  free(path);

  while (1) {
    // Wait for moving
    semaphore_op(semid, dev_num, -1);

    // Check if we need to send messages
    if (messages->length > 0) {
      // Access acknowledgement and board shared memory
      semaphore_op(semid, BOARD_SEM, -1);
      semaphore_op(semid, ACK_SEM, -1);

      list_message *s_list =
          check_send_messages(pid, dev_num, position, messages);
      for (node_message *n_message = s_list->head; n_message != NULL;
           n_message = n_message->next) {
        send_message_device(&n_message->value);
      }
      free_list_message(s_list);

      // Unlock the acknowledgement and boar
      semaphore_op(semid, BOARD_SEM, 1);
      semaphore_op(semid, ACK_SEM, 1);
    }

    // Read messages
    recv_messages_device(fifo, dev_num, messages);

    // Move the device only if the next position is empty, otherwise resets the
    // position
    semaphore_op(semid, ACK_SEM, -1);
    if (shm_board(shm_positions[dev_num].i, shm_positions[dev_num].j) == 0) {
      shm_board(position.i, position.j) = 0;
      COPY_POSITION(position, shm_positions[dev_num]);
      shm_board(position.i, position.j) = pid;
    } else {
      COPY_POSITION(position, shm_positions[dev_num]);
    }
    semaphore_op(semid, ACK_SEM, 1);

    // Move next device
    if (dev_num + 1 < DEVICE_NUMBER) {
      semaphore_op(semid, dev_num + 1, 1);
    }
  }
}

list_message *check_send_messages(pid_t pid, uint8_t dev_num, vec_2 position,
                                  list_message *list) {
  list_message *send_list = create_list_message(NULL, NULL, 0);
  node_message *node;
  node_message *n_message;
  bool msg_sent;

  // Cycle throw the messages
  node = list->head;
  while (node != NULL) {

    // Check for acknowledgement of the message, otherwise remove it
    if (shm_ack(node->value.message_id, dev_num).message_id ==
        node->value.message_id) {
      msg_sent = false;

      // Check the board for a device, get the device number and check if we
      // need to send a message to it and then send it
      for (uint8_t i = 0; i < BOARD_SIZE && !msg_sent; i++) {
        for (uint8_t j = 0; j < BOARD_SIZE && !msg_sent; j++) {

          // If there is the PID of a device on the board
          if (shm_board(i, j) != 0 && shm_board(i, j) != pid) {
            // Device number
            uint8_t d_n = 0;
            while (shm_dev[d_n] != shm_board(i, j))
              d_n++;

            // Check if we need to send a message
            if (shm_ack(node->value.message_id, d_n).message_id !=
                node->value.message_id) {

              // Check if the distance is inferior to max distance
              if (distance_sqr(position, (vec_2){i, j}) <=
                  node->value.max_distance) {

                // Add message to send list
                n_message = malloc(sizeof(node_message));
                memcpy(&n_message->value, &node->value, sizeof(Message));
                n_message->value.pid_sender = pid;
                n_message->value.pid_receiver = shm_dev[d_n];
                append_list_message(send_list, n_message);
                n_message = NULL;

                // Set msg_sent to pass to the next message
                msg_sent = true;
              }
            }
          }
        }
      }

      // Next message
      node = node->next;
    } else {
      node_message *t_m = node;
      node = node->next;
      remove_list_message(list, t_m);
    }
  }

  return send_list;
}

void recv_messages_device(int fifo, uint8_t dev_num, list_message *list) {
  node_message *node = NULL;
  ssize_t b_read;

  // Read all messages
  do {
    // Allocate message only if needed
    if (node == NULL) {
      node = malloc(sizeof(node_message));
    }

    // Read a message
    b_read = read(fifo, &node->value, sizeof(Message));

    // Check for errors
    if (b_read == -1)
      err_exit("read", __FILE__, __LINE__);

    // Check if the message is the right size, if not there was an error.
    if (b_read == sizeof(Message)) {

      // Check if message is in message list
      bool msg_found = false;
      for (node_message *msg = list->head; msg != NULL && !msg_found;
           msg = msg->next) {
        if (msg->value.message_id == node->value.message_id)
          msg_found = true;
      }

      // If message was not found add it to the list and write an
      // acknowledgement
      if (!msg_found) {
        append_list_message(list, node);

        // Access acknowledgement shared memory
        semaphore_op(semid, ACK_SEM, -1);

        // Set the acknowledgement
        shm_ack(node->value.message_id, dev_num).message_id =
            node->value.message_id;
        shm_ack(node->value.message_id, dev_num).pid_sender =
            node->value.pid_sender;
        shm_ack(node->value.message_id, dev_num).pid_receiver =
            node->value.pid_receiver;
        shm_ack(node->value.message_id, dev_num).timestamp = time(NULL);

        // Unlock the acknowledgement
        semaphore_op(semid, ACK_SEM, 1);
        node = NULL;
      }

    } else if (b_read > 0) {
      print_err("read: Not the size of Message", __FILE__, __LINE__);
    }

  } while (b_read == sizeof(Message));

  // Free the node if still allocated
  if (node != NULL)
    free(node);
}
