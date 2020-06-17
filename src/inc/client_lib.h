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
Message *create_message_client();

/**
 * Calculates the square of the max distance, it handles overflows setting the
 * max distance to the max distance on the board plus 1.
 *
 * @pram max_distance The double version of max distance
 * @return The square of max_distance
 */
double max_distance_sqr(double max_distance);

/**
 * Writes the list of acknowledgment to the file `out_<message_id>.txt`
 *
 * @param message Message sent to the devices
 * @param ack_list List of acknowledgement
 */
void write_out_message_id(Message *message, Acknowledgment ack_list[5]);
