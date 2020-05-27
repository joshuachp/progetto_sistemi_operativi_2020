/// @file semaphore.h
/// @brief Contiene la definizioni di variabili e funzioni
///         specifiche per la gestione dei SEMAFORI.

#pragma once

#include "position.h"

/**
 * Read the position file passed to the server
 *
 * @param filename Path to the file to read
 * @return A matrix with 5 vec_2 each and a final NULL element
 */
vec_2 **read_positions_file(char *filename);
