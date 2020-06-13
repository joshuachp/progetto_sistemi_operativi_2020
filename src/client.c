/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "client_lib.h"
#include "defines.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Get key argument
  uint32_t key;
  if (argc != 3 || sscanf(argv[1], "%u", &key) != 1) {
    print_help_client();
    return 1;
  }

  pid_t pid;
  Message *message = create_message_client(&pid);

  // Exit
  free(message);
  return 0;
}
