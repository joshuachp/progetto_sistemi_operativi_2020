/// @file server.c
/// @brief Contiene l'implementazione del SERVER.

#include "ack_manager.h"
#include "defines.h"
#include "device.h"
#include "err_exit.h"
#include "files.h"
#include "position.h"
#include "server_lib.h"
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  // Get parameters
  key_t key;
  if (argc != 3 || sscanf(argv[1], "%d", &key) != 1) {
    print_help_server();
    return 1;
  }
  char *file = argv[2];

  list_positions *positions = read_positions_file(file);

  // Server setup
  set_up_server(key);

  // Fork ack manager
  pid_t pid = fork();
  if (pid == -1)
    err_exit("fork", __FILE__, __LINE__);
  if (pid == 0) {
    // ack manager
    ack_manager_process();
    return 0;
  }
  pid_ack = pid;

  // Fork devices
  for (uint8_t i = 0; i < DEVICE_NUMBER; i++) {
    pid = fork();
    if (pid == -1)
      err_exit("fork", __FILE__, __LINE__);
    if (pid == 0) {
      // device
      device_process(i);
      return 0;
    }
    shm_dev[i] = pid;
  }

  // Signals setup
  setup_sig_handler();

  // server processes
  server_process(positions);

  // Free structures
  // XXX: Unreachable if program is executed normally
  free_list_positions(positions);
  return 0;
}
