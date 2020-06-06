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
  // XXX: Testing
  list_positions *positions = read_positions_file(file);
  free_list_positions(positions);

  // Signals setup
  setup_sig_handler();

  return 0;
}
