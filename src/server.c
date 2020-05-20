/// @file server.c
/// @brief Contiene l'implementazione del SERVER.

#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include "semaphore.h"
#include "shared_memory.h"
#include <stdio.h>

/*
 * Prints the help for the binary
 */
void print_help(void);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    print_help();
    return 1;
  }

  return 0;
}

void print_help(void) {
  puts("Usage:\n"
       "  server KEY FILE\n"
       "\n"
       "With the KEY of the message queue and FILE for the positions.");
}
