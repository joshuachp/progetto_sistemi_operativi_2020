/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "defines.h"
#include <stdio.h>

/*
 * Prints the help for the binary
 */
void print_help(void);

int main(int argc, char *argv[]) {
  // Key argument is required
  if (argc != 2) {
    print_help();
    return 1;
  }
  return 0;
}

void print_help() {
  puts("Usage:\n"
       "  client KEY\n"
       "\n"
       "With the KEY of the message queue.");
}
