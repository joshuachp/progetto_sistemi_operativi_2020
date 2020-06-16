/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "client_lib.h"
#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  // Get key argument
  key_t key;
  if (argc != 2 || sscanf(argv[1], "%d", &key) != 1) {
    print_help_client();
    return 1;
  }

  // Get message queue
  int msqid = msgget(key, S_IRUSR | S_IWUSR | S_IRGRP);
  if (msqid == -1)
    err_exit("msgget", __FILE__, __LINE__);

  // Create and sends a message to a device
  Message *message = create_message_client();
  send_message_device(message);

  // Receive acknowledgment
  ack_msg *ack = malloc(sizeof(ack_msg));
  if (msgrcv(msqid, ack, sizeof(ack->ack_list), message->message_id, 0))
    err_exit("msgrcv", __FILE__, __LINE__);

  // Write to out file
  write_out_message_id(message, ack->ack_list);

  // Free structures
  free(message);
  free(ack);
  return 0;
}
