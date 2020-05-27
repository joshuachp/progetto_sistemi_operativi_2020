/// @file files.h
/// @brief Contiene la definizioni di variabili e funzioni
///         specifiche per la gestione files.

#pragma once

#include "position.h"
#include <stdlib.h>

/**
 * Read the position file passed to the server.
 *
 * @param filename Path to the file to read
 * @return A matrix of 5 vec_2 with NUll at the end, NULL on error
 */
vec_2 **read_positions_file(char *filename);

/**
 * Split a string in array of five positions(vec_2). The string has to be the
 * correct format.
 *
 * @param str Sting to split
 * @return Array of 5 vec_2, NULL on error
 */
vec_2 *str_to_position_array(char *);

/**
 * Prints error parsing positions string to stderr
 *
 * @return Return error as -1
 */
void *error_parsing_positions(char *);

/**
 * Transform a string buf in an array of strings for each line in the bug.
 *
 * @param buf The buffer string
 * @return Array of lines, NULL on error
 */
char **get_lines_buf_positions(char *);

/**
 * Add an array to the positions, if the element is the last it will double the
 * size of the positions matrix.
 *
 * @param positions Matrix of the positions
 * @param array Array to add
 * @param index Index to add the array
 * @param size First size of the matrix
 * @return Pointer to the matrix
 */
vec_2 **add_array_to_positions(vec_2 **, vec_2 *, size_t *, size_t *);
