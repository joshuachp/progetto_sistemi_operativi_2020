/// @file server.c
/// @brief Contiene l'implementazione del CLIENT.

#include "client_lib.h"
#include "defines.h"
#include "err_exit.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void print_help_client() {
  puts("Usage:\n"
       "  client KEY\n"
       "\n"
       "With the KEY of the message queue.");
}

Message *create_message_client() {
  Message *message = malloc(sizeof(Message));

  // Set the sender PID
  message->pid_sender = getpid();

  // Set the device PID
  printf("PID of the device: ");
  while (scanf("%d", &message->pid_receiver) != 1 ||
         message->pid_receiver < 0) {
    puts("Please enter a valid PID.");
    printf("PID of the device: ");
  }

  // Set the message ID
  printf("Message id: ");
  while (scanf("%d", &message->message_id) != 1 || message->message_id < 0 ||
         message->message_id >= ACK_SIZE) {
    printf("Please enter a valid ID [0,%d].\n", ACK_SIZE - 1);
    printf("Message ID: ");
  }

  char t;
  scanf("%c", &t);
  // Set the message content
  printf("Message content: ");
  while (scanf("%255[^\n]", message->message) != 1) {
    puts("Please enter a valid content.");
    printf("Message content: ");
  }

  printf("Message max distance: ");
  while (scanf("%lf", &message->max_distance) != 1 &&
         message->max_distance < 0) {
    printf("Message ID: ");
    puts("Please enter a valid max distance.");
  }
  // We square the value
  message->max_distance *= message->max_distance;
  return message;
}

void write_out_message_id(Message *message,
                          Acknowledgment ack_list[DEVICE_NUMBER]) {
  int length;
  char *str = malloc(STRING_ALLOC_SIZE);
  char time_buf[TIME_BUF_SIZE];

  // Format file path
  if (snprintf(str, STRING_ALLOC_SIZE, "out_%d.txt", message->message_id) == -1)
    err_exit("snprintf", __FILE__, __LINE__);

  // Open file
  int fd = open(str, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
  if (fd == -1)
    err_exit("open", __FILE__, __LINE__);

  // Write message and message content, get the length of the formatted message
  // for the write
  length = snprintf(str, STRING_ALLOC_SIZE,
                    "Messaggio %d: %s\n"
                    "Lista acknowledgment:\n",
                    message->message_id, message->message);
  if (length == -1)
    err_exit("snprintf", __FILE__, __LINE__);

  // Write to file message to file
  if (write(fd, str, length) == -1)
    err_exit("write", __FILE__, __LINE__);

  // Write acknowledgment list
  for (int i = 0; i < 5; i++) {
    // Format the time stamp
    if (strftime(time_buf, TIME_BUF_SIZE, "%Y-%m-%d %H:%M:%S",
                 localtime(&ack_list[i].timestamp)) == 0)
      err_exit("strftime", __FILE__, __LINE__);

    // Get the length of the list
    length =
        snprintf(str, STRING_ALLOC_SIZE, "%d, %d, %s\n", ack_list[i].pid_sender,
                 ack_list[i].pid_receiver, time_buf);
    if (length == -1)
      err_exit("snprintf", __FILE__, __LINE__);

    // Write to the file the list
    if (write(fd, str, length) == -1)
      err_exit("write", __FILE__, __LINE__);
  }

  // Close file
  if (close(fd == -1))
    err_exit("close", __FILE__, __LINE__);

  // Free
  free(str);
}
