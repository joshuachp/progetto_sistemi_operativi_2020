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
  // Create a message
  pid_t pid;
  Message *message = create_message_client(&pid);
  // Open FIFO device
  char *path = pid_fifo_path(pid);
  int fd = open(path, O_WRONLY);
  if (fd == -1)
    err_exit("open", __FILE__, __LINE__);
  // Write message
  if (write(fd, message, sizeof(Message)) < (ssize_t)sizeof(Message))
    err_exit("write", __FILE__, __LINE__);
  // Close FIFO device
  if (close(fd) == -1)
    err_exit("close", __FILE__, __LINE__);
  // Receive acknowledgment
  ack_msg *ack = malloc(sizeof(ack_msg));
  // use PID for distingue message type
  if (msgrcv(msqid, ack, sizeof(ack->ack), getpid(), 0))
    err_exit("msgrcv", __FILE__, __LINE__);
  // Free structures
  free(message);
  free(path);
  free(ack);
  return 0;
}
