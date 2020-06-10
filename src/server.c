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

  // Signals setup
  setup_sig_handler();

  // Server setup
  set_up_server();

  // Setup processes
  pid_t pid;
  pid_t *pid_devices = malloc(sizeof(pid_t) * DEVICE_NUMBER);
  // Fork ack manager
  pid = fork();
  if (pid == -1)
    err_exit("fork");
  if (pid == 0) {
    // ack manager
  }

  // Fork devices
  for (int i = 0; i < 5; i++) {
    pid = fork();
    if (pid == -1)
      err_exit("fork");
    if (pid == 0) {
      // device
    }
    pid_devices[i] = pid;
  }

  // server processes
  server_process(positions);
  free_list_positions(positions);

  return 0;
}

