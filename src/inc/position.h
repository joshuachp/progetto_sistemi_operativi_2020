/// @file position.h
/// @brief Contiene la definizione delle funzioni e strutture per la posizione
///        sulla scacchiera

#include <stdint.h>

#pragma once

/*
 * Struct for the tuple of position on the board
 */
typedef struct vec_2 {
  uint8_t i;
  uint8_t j;
} vec_2;

/*
 * Square value of the Euclid distance of two points on the board
 *
 * @param arg1 Point a
 * @param arg2 Point b
 * @return Distance from a to b
 */
uint8_t distance_sqr(vec_2 a, vec_2 b);
