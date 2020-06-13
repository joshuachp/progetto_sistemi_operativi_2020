/// @file fifo.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione delle FIFO.

#include "fifo.h"
#include "err_exit.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *pid_fifo_path(pid_t pid) {
  // Get length of the final string for allocation
  int length = snprintf(NULL, 0, "/tmp/devices/dev_fifo.%d", pid);
  if (length == -1)
    print_perror("snprintf", __FILE__, __LINE__);
  char *path = malloc(length + 1);
  if (sprintf(path, "/tmp/devices/dev_fifo.%d", pid) != length)
    print_perror("sprintf", __FILE__, __LINE__);
  return path;
}

void make_fifo_device(pid_t pid) {
  char *path = pid_fifo_path(pid);
  if (mkfifo(path, S_IWUSR | S_IRUSR | S_IWGRP) == -1)
    err_exit("mkfifo", __FILE__, __LINE__);
  free(path);
}

void remove_fifo_device(pid_t pid) {
  char *path = pid_fifo_path(pid);
  if (remove(path) == -1)
    err_exit("remove", __FILE__, __LINE__);
  free(path);
}
