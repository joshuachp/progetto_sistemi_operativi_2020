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
  vec_2 *array;
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
  char **lines;
  size_t j;
  while (b_read != 0) {
    // Setts last byte to 0 for strings functions
    buf[b_read] = 0;
    // Gets each line
    lines = get_lines_buf_positions(buf);
    j = 0;
    while (lines[j] != NULL) {
      array = str_to_position_array(lines[j]);
      if (array == NULL) {
        free(positions);
        free(lines);
        free(buf);
        return NULL;
      }
      positions = add_array_to_positions(positions, array, &i, &p_length);
      j++;
    }
    free(lines);
    // Read next chunk
    b_read = read(fd, buf, BUF_READ_SIZE - 1);
    if (b_read == -1)
      err_exit("Error read position file");
  }
  free(buf);
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

char **get_lines_buf_positions(char *buf) {
  size_t i = 0;
  // Find first character that is not a new line character
  while (buf[i] == '\n')
    i++;
  // If reached end return null
  if (buf[i] == 0)
    return NULL;
  // Lines are dynamically allocated
  char **lines = calloc(MIN_POSITIONS_ARRAY_LENGTH, sizeof(char *));
  size_t l_length = MIN_POSITIONS_ARRAY_LENGTH;
  size_t count = 0;
  // Initialize firs string
  lines[count] = &buf[i];
  count++;
  // Cycle all the characters
  while (buf[i] != 0) {
    // Check if it's a new line to close previews string
    if (buf[i] == '\n') {
      buf[i] = 0;
      i++;
      // Find first character that is not a new line character
      while (buf[i] == '\n') {
        i++;
      }
      // If reached the end returns
      if (buf[i] == 0)
        return lines;
      // Set new string
      lines[count] = &buf[i];
      count++;
      lines[count] = NULL;
      // If last element is reached double the array dimension
      if (count == MIN_POSITIONS_ARRAY_LENGTH - 1) {
        l_length *= 2;
        lines = realloc(lines, l_length * sizeof(char *));
      }
    }
    // Increment first wile
    i++;
  }
  return lines;
}

vec_2 **add_array_to_positions(vec_2 **positions, vec_2 *array, size_t *index,
                               size_t *size) {
  positions[*index] = array;
  // Increment i and check if its the last element, if it is double the
  // size of the array
  *index += 1;
  if (*index == *size - 1) {
    *size *= 2;
    positions = realloc(positions, *size * sizeof(vec_2 *));
  }
  // Sets last element to NULL
  positions[*index] = NULL;
  return positions;
}
