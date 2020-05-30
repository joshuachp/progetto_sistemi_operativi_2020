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
 * @return A list of position, NULL on error
 */
list_positions *read_positions_file(char *filename);

/**
 * Parse a string into an array of 5 vec_2
 *
 * @param str Sting to split
 * @return Array of 5 vec_2, NULL on error
 */
node_positions *parse_position_str(char *);

/**
 * Transform a string buf in an array of strings for each line in the bug.
 *
 * @param buf The buffer string
 * @param start The buffer string
 * @return Next line in buffer
 */
char *get_next_line_buf(char *, size_t);
