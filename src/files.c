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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

list_positions *read_positions_file(char *filename) {
  // XXX: mmap to read file has better performance but is more unstable. We
  // could also use the information of the file size to read the file in one go
  // but it will have the same effect of the mmap.
  // Use read in release
#ifndef NDEBUG
  // Buffer
  int fd = open(filename, O_RDONLY, S_IRUSR | S_IRGRP);
  if (fd == -1) {
    err_exit("open", __FILE__, __LINE__);
  }
  // File stats
  struct stat sb;
  if (fstat(fd, &sb) == -1)
    err_exit("fstat", __FILE__, __LINE__);

  // Check size if null return
  if (sb.st_size == 0) {
    fprintf(stderr,
            "Error in file \"%s\" at line %d\n"
            "stat: file is empy\n",
            __FILE__, __LINE__);
    return NULL;
  }

  // Map file to buff
  char *buf =
      mmap(NULL, sb.st_size + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (buf == MAP_FAILED)
    err_exit("mmap", __FILE__, __LINE__);
  // make buffer a string
  buf[sb.st_size] = '0';
  // Close file
  if (close(fd) == -1)
    err_exit("close", __FILE__, __LINE__);

  // Create positions list
  list_positions *positions = create_list_positions(NULL, NULL, 0);

  // Lines
  size_t index = 0;
  char *line;
  node_positions *node;
  while (index != (size_t)sb.st_size && buf[index] != 0) {
    line = get_next_line_buf(buf, &index);
    if (line == NULL) {
      free(line);
      munmap(buf, sb.st_size + 1);
      free_list_positions(positions);
      return NULL;
    }

    // Parse line
    node = parse_position_str(line);
    if (node == NULL) {
      free(line);
      munmap(buf, sb.st_size + 1);
      free_list_positions(positions);
      return NULL;
    }

    // Append to positions
    append_list_positions(positions, node);

    // Next line
    index += 1;

    // Exit while
    free(line);
    line = NULL;
  }

  munmap(buf, sb.st_size + 1);
  return positions;
#else
  // Buffer
  int fd = open(filename, O_RDONLY, S_IRUSR | S_IRGRP);
  if (fd == -1) {
    err_exit("open");
  }
  // File stats
  struct stat sb;
  if (fstat(fd, &sb) == -1)
    err_exit("Error fstat");

  // Check size if null return
  if (sb.st_size == 0) {
    fprintf(stderr,
            "Error in file \"%s\" at line %d\n"
            "stat: file is empy\n",
            __FILE__, __LINE__);
    return NULL;
  }

  // Map file to buff
  char *buf = malloc(BUF_READ_SIZE + 1);
  size_t file_read = 0;

  // Create positions list
  list_positions *positions = create_list_positions(NULL, NULL, 0);

  // Lines
  size_t index;
  size_t n_index;

  char *line;
  node_positions *node;

  size_t b_read = read(fd, buf, BUF_READ_SIZE);
  while (b_read != 0) {
    // Make buf a string
    buf[b_read] = 0;
    file_read += b_read;

    index = 0;
    n_index = 0;

    while (n_index < b_read) {
      line = get_next_line_buf(buf, &n_index);
      if (line == NULL) {
        free(line);
        free(buf);
        free_list_positions(positions);
        return NULL;
      }

      if (n_index >= b_read && file_read < sb.st_size) {
        file_read -= n_index - index;
        if (lseek(fd, file_read, SEEK_SET) == -1) {
          free(line);
          free(buf);
          free_list_positions(positions);
          err_exit("lseek");
        }
        break;
      }

      // Parse line
      node = parse_position_str(line);
      if (node == NULL) {
        free(line);
        free(buf);
        free_list_positions(positions);
        return NULL;
      }

      // Append to positions
      append_list_positions(positions, node);

      // Next line
      n_index += 1;
      index = n_index;

      free(line);
      line = NULL;
    }

    // Exit while
    b_read = read(fd, buf, BUF_READ_SIZE - 1);
  }

  free(buf);
  return positions;
#endif
}

char *get_next_line_buf(char *buf, size_t *index) {
  // Get starting string
  char *str = &buf[*index];
  if (str == 0) {
    fprintf(stderr,
            "Error in file \"%s\" at line %d\n"
            "get_next_line_buf: string is empty\n",
            __FILE__, __LINE__);
    return NULL;
  }

  // Find next new line
  char *end = str;
  while (*end != 0 && *end != '\n') {
    (*index) += 1;
    end += 1;
  }

  // Copy the string
  char *ret = malloc(end - str);
  char t = *end;
  *end = 0;
  ret = strcpy(ret, str);
  *end = t;

  return ret;
}

node_positions *parse_position_str(char *str) {
  if (str == 0 || strlen(str) != 19) {
    fprintf(stderr,
            "Error in file \"%s\" at line %d\n"
            "parse_position_str: wrong string length\n",
            __FILE__, __LINE__);
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
    fprintf(stderr,
            "Error in file \"%s\" at line %d\n"
            "parse_position_str: string malformed\n",
            __FILE__, __LINE__);
    free(node);
    return NULL;
  }
  node->next = NULL;
  node->prev = NULL;
  return node;
}
