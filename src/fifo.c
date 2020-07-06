/// @file fifo.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione delle FIFO.

#include "fifo.h"
#include "defines.h"
#include "err_exit.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *pid_fifo_path(pid_t pid) {
  // Get length of the final string for allocation
  char *path = malloc(STRING_ALLOC_SIZE);
  if (snprintf(path, STRING_ALLOC_SIZE, "/tmp/dev_fifo.%d", pid) == -1)
    print_perror("snprintf", __FILE__, __LINE__);
  return path;
}

void make_fifo_device(pid_t pid) {
  char *path = pid_fifo_path(pid);
  if (mkfifo(path, S_IWUSR | S_IRUSR | S_IRGRP) == -1)
    err_exit("mkfifo", __FILE__, __LINE__);
  free(path);
}

void remove_fifo_device(pid_t pid) {
  char *path = pid_fifo_path(pid);
  if (remove(path) == -1)
    print_perror("remove", __FILE__, __LINE__);
  free(path);
}
