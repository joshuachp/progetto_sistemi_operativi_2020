/// @file server.c
/// @brief Contiene l'implementazione del CLIENT.

#include "client_lib.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void print_help_client() {
  puts("Usage:\n"
       "  client KEY\n"
       "\n"
       "With the KEY of the message queue.");
}

Message *create_message_client(pid_t *pid) {
  Message *message = malloc(sizeof(Message));
  // Set the sender PID
  message->pid_sender = getpid();
  // Set the device PID
  printf("PID of the device: ");
  while (scanf("%d", pid) != 1 || *pid < 0) {
    puts("Please enter a valid PID.");
    printf("PID of the device: ");
  }
  message->pid_receiver = *pid;
  // Set the message ID
  printf("Message id: ");
  while (scanf("%d", &message->message_id) != 1 || message->message_id < 0) {
    puts("Please enter a valid ID.");
    printf("Message ID: ");
  }
  // Set the message content
  printf("Message content: ");
  while (scanf("%255s", message->message) != 1) {
    puts("Please enter a valid content.");
    printf("Message ID: ");
  }
  // XXX: We ask for a double value, but we convert it in uint8_t since the
  //      squared maximum value can only be 10^2+10^2=200
  // Set the maximum distance
  double max;
  printf("Message max distance: ");
  while (scanf("%lf", &max) != 1 && max < 0) {
    printf("Message ID: ");
    puts("Please enter a valid max distance.");
  }
  message->max_distance = max_distance_sqr(max);
  return message;
}

uint8_t max_distance_sqr(double max_distance) {
  // Overflow and other control for conversion
  if (200 / max_distance <= 14.142135624) {
    return 200;
  }
  double m = max_distance * max_distance + 1e-8;
  return m;
}
