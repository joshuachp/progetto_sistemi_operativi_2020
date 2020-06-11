/// @file server.c
/// @brief Contiene l'implementazione del SERVER.

#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include "files.h"
#include "position.h"
#include "semaphore.h"
#include "server_lib.h"
#include "shared_memory.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  uint32_t key;
  if (argc != 3 || sscanf(argv[1], "%u", &key) != 1) {
    print_help();
    return 1;
  }
  char *file = argv[2];
  list_positions *positions = read_positions_file(file);

  // Server setup
  set_up_server();

  // Fork ack manager
  pid_ack = fork();
  if (pid_ack == -1)
    err_exit("fork");
  if (pid_ack == 0) {
    // ack manager
  }

  // Fork devices
  for (int i = 0; i < DEVICE_NUMBER; i++) {
    pid_devices[i] = fork();
    if (pid_devices[i] == -1)
      err_exit("fork");
    if (pid_devices[i] == 0) {
      // device
    }
  }

  // Signals setup
  setup_sig_handler();

  // server processes
  server_process(positions);

  // Free structures
  free_list_positions(positions);
  return 0;
}

