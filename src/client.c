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
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  // Get key argument
  uint32_t key;
  if (argc != 2 || sscanf(argv[1], "%u", &key) != 1) {
    print_help_client();
    return 1;
  }

  pid_t pid;
  Message *message = create_message_client(&pid);
  char *path = pid_fifo_path(pid);

  int fd = open(path, O_WRONLY);
  if (fd == -1)
    err_exit("open", __FILE__, __LINE__);

  // Exit
  free(message);
  free(path);

  if (close(fd) == -1)
    err_exit("close", __FILE__, __LINE__);

  return 0;
}
