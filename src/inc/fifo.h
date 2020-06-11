/// @file fifo.h
/// @brief Contiene la definizioni di variabili e
///         funzioni specifiche per la gestione delle FIFO.

#pragma once

#include <sys/types.h>

/**
 * Create a fifo for a device
 */
void make_fifo_device(pid_t pid);
