/// @file files.c
/// @brief Contiene l'implementazione delle funzioni per la gestione dei files.

#include "files.h"
#include "defines.h"
#include "err_exit.h"
#include "position.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

vec_2 **read_positions_file(char *filename) {
  // Open the file read-only
  int fd = open(filename, O_RDONLY, S_IRUSR | S_IRGRP);
  if (fd == -1) {
    err_exit("open");
  }
  // Creates the position array of certain size p_length that will be doubled
  // every time we reach the last element
  size_t p_length = MIN_POSITIONS_ARRAY_LENGTH;
  vec_2 **positions = calloc(p_length, sizeof(vec_2 *));
  size_t i = 0;
  // Buffer for the read
  char *buf = calloc(BUF_READ_SIZE, sizeof(char));
  size_t b_read = read(fd, buf, BUF_READ_SIZE - 1);
  if (b_read == -1)
    err_exit("Error read position file");
  else if (b_read == 0) {
    fputs("Error positions file is empty", stderr);
    return NULL;
  }
  // Lines of the buff
  char *line;
  while (b_read != 0) {
    // Setts last byte to 0 for strings functions
    buf[b_read] = 0;
    // Gets each line
    line = strtok(buf, "\n");
    while (line != NULL) {
      positions[i] = str_to_position_array(line);
      // Error parsing line
      if (positions[i] == NULL)
        return NULL;
      // Increment i and check if its the last element, if it is double the
      // size of the array
      i++;
      if (i == p_length - 1) {
        p_length *= 2;
        positions = realloc(positions, p_length * sizeof(vec_2 *));
      }
      // Sets last element to NULL
      positions[i] = NULL;

      line = strtok(NULL, "\n");
    }

    b_read = read(fd, buf, BUF_READ_SIZE - 1);
    if (b_read == -1)
      err_exit("Error read position file");
  }
  return positions;
}

vec_2 *str_to_position_array(char *str) {
  // Check if the string is NULL or has enough length for 5 positions
  if (str == NULL || strlen(str) != 19) {
    return error_parsing_positions(str);
  }
  char *str_pos = strdup(str);
  vec_2 *vec = calloc(5, sizeof(vec_2));
  uint8_t count = 0;
  uint32_t i, j;
  // Divide the string for each separator `|`
  char *part = strtok(str_pos, "|");
  while (part != NULL) {
    // Checks if it contains two unsigned integer that are on the board
    if (sscanf(part, "%u,%u", &i, &j) != 2 || i >= 10 || j >= 10) {
      free(str_pos);
      free(vec);
      return error_parsing_positions(str);
    }
    vec[count].i = i;
    vec[count].j = j;

    count++;
    part = strtok(NULL, "|");
  }
  // Returns error if there aren't 5 elements
  if (count != 5) {
    free(str_pos);
    free(vec);
    return error_parsing_positions(str);
  }
  free(str_pos);
  return vec;
}

void *error_parsing_positions(char *str) {
  fprintf(stderr, "Error parsing positions: %s\n", str);
  return NULL;
}

char **get_lines_buf(char *buf, size_t buf_size) {
  size_t i;
  size_t count = 0;
  // Count the number of new lines
  for (i = 0; i < buf_size; i++) {
    if (buf[i] == '\n')
      count++;
  }
  // Not consider last newline
  if (buf[buf_size - 1] == '\n')
    count--;
  // Allocate the array plus the NULL terminator
  char **lines = calloc(count + 1, sizeof(char *));
  count = 0;
  lines[count] = buf;
  for (i = 0; i < buf_size - 1; i++) {
    // If encounters a newline replace it and starts the new string
    if (buf[i] == '\n') {
      buf[i] = 0;
      count++;
      lines[count] = &buf[i + 1];
    }
  }
  // Check last char for newline
  if (buf[i] == '\n')
    buf[i] = 0;
  return lines;
}
