/// @file fifo.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione delle FIFO.

#include "fifo.h"
#include "err_exit.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void make_fifo_device(pid_t pid) {
  // Get length of the final string for allocation
  int length = snprintf(NULL, 0, "/tmp/devices/dev_fifo.%d", pid);
  char *path = malloc(length + 1);
  if (sprintf(path, "/tmp/devices/dev_fifo.%d", pid) != 1)
    fputs("Error sprintf in make_fifo", stderr);
  if (mkfifo(path, S_IWUSR | S_IRUSR | S_IWGRP) == -1)
    err_exit("failed");
}
