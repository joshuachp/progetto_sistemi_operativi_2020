/// @file fifo.h
/// @brief Contiene la definizioni di variabili e
///         funzioni specifiche per la gestione delle FIFO.

#pragma once

#include <sys/types.h>

/**
 * Allocate a string for the file path of the FIFO file of a specific device,
 * the string needs to be deallocated. The max FIFO path size is 1024 bits.
 *
 * @param pid PID of the device
 */
char *pid_fifo_path(pid_t pid);

/**
 * Create a FIFO for a device, the path will be `/tmp/devices/dev_fifo.pid`
 *
 * @param pid PID of the device
 */
void make_fifo_device(pid_t pid);

/**
 * Deletes a FIFO for a device
 *
 * @param pid PID of the device
 */
void remove_fifo_device(pid_t pid);
