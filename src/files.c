/// @file files.c
/// @brief Contiene l'implementazione delle funzioni per la gestione dei files.

#include "files.h"
#include "defines.h"
#include "err_exit.h"
#include "position.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// FIXME: Complete implementation
vec_2 **read_positions_file(char *filename) {
  // Buffer
  int fd = open(filename, O_RDONLY, S_IRUSR | S_IRGRP);
  if (fd == -1) {
    err_exit("open");
  }
  // File stats
  struct stat sb;
  if (fstat(fd, &sb) == -1)
    err_exit("Error fstat");
  // Map file to buff
  char *buf =
      mmap(NULL, sb.st_size + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (buf == MAP_FAILED)
    err_exit("Error mmap");
  // make buffer a string
  buf[sb.st_size] = '0';
  // Close file
  if (close(fd) == -1)
    err_exit("Error close");

  // Posizioni

  // Lines
  size_t index = 0;
  char *line;
  while (index != sb.st_size) {
    line = get_next_line_buf(buf, index);

    // Parse line

    // TODO: Add to positions

    // Next line
    index += strlen(line + 1);

    // Exit while
    free(line);
    line = NULL;
  }

  munmap(buf, sb.st_size + 1);
  return NULL;
}

char *get_next_line_buf(char *buf, size_t start) {
  // Get starting string
  char *str = &buf[start];
  if (str == 0) {
    return NULL;
  }

  // Find next new line
  char *end = str;
  while (*end != 0 && *end != '\n')
    end += 1;

  // Copy the string
  char *ret = malloc(sizeof(char) * (end - str));
  char t = *end;
  *end = 0;
  ret = strcpy(ret, str);
  *end = t;

  return ret;
}

node_positions *parse_position_str(char *str) {
  if (str == 0 || strlen(str) != 19) {
    return NULL;
  }
  node_positions *node = malloc(sizeof(node_positions));
  // Use sscanf to parse the whole string, the format is 8 bit unsigned int
  int check = sscanf(str, "%hhu,%hhu|%hhu,%hhu|%hhu,%hhu|%hhu,%hhu|%hhu,%hhu",
                     &node->value[0].i, &node->value[0].j, &node->value[1].i,
                     &node->value[1].j, &node->value[2].i, &node->value[2].j,
                     &node->value[3].i, &node->value[3].j, &node->value[4].i,
                     &node->value[4].j);
  if (check != 10) {
    free(node);
    return NULL;
  }
  node->next = NULL;
  node->prev = NULL;
  return node;
}
