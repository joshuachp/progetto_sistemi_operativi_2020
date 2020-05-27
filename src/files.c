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
  int fd = open(filename, O_RDONLY | S_IRUSR);
  if (fd == -1) {
    err_exit("open");
  }
  return NULL;
};

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
      return error_parsing_positions(part);
    }
    vec[count].i = i;
    vec[count].j = j;

    count++;
    part = strtok(NULL, "|");
  }
  // Returns error if there aren't 5 elements
  if (count != 5) {
    return error_parsing_positions(str);
  }
  return vec;
}

void *error_parsing_positions(char *str) {
  fprintf(stderr, "Error parsing positions: %s\n", str);
  return (void *)-1;
}
