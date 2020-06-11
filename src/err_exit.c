/// @file err_exit.c
/// @brief Contiene l'implementazione della funzione di stampa degli errori.

#include "err_exit.h"
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void err_exit(const char *msg, char *file, int line) {
  fprintf(stderr, "Error in file \"%s\" at line %d\n", file, line);
  perror(msg);
  exit(1);
}
