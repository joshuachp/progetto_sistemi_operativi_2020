/// @file position.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la posizione
/// sulla scacchiera

#include "position.h"
#include <stdio.h>

uint8_t distance_sqr(vec_2 a, vec_2 b) {
  // Difference of the two
  int8_t j = a.j - b.j;
  int8_t i = a.i - b.i;
  // Return the squared sum
  return i * i + j * j;
}
