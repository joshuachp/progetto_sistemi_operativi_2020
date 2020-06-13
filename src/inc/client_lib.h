/// @file server_lib.h
/// @brief  Contiene la definizioni di variabili e funzioni specifiche per il
///         CLIENT.

#pragma once

#include "defines.h"
#include <stdint.h>
#include <sys/types.h>

/**
 * Prints the help for the CLIENT
 */
void print_help_client(void);

/**
 * Get input for creating a message to send to a device. It will ask to the
 * client for: the PID of the device, an unique id for the message, the message
 * and the maximum distance for the communication of the message.
 *
 * @param pid PID of the device to send the message to.
 * @return Message struct with the data inputed by the user
 */
Message *create_message_client(pid_t *pid);

/**
 * Approximate max distance, converts a double max distance in its uint8_t
 * squared version. Separated from the main function for test.
 *
 * @pram max_distance The double version of max distance
 * @return The uint8_t square of max_distance
 */
uint8_t max_distance_sqr(double max_distance);

