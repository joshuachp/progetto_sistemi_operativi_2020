/// @file device.h
/// @brief Contains the declaration of function and variables for the device..

#pragma once

#include "defines.h"
#include "position.h"
#include <stdint.h>
#include <sys/types.h>

/**
 * Function for the devices, create a FIFO
 *
 * @param dev_num Device number for semaphores
 */
void device_process(uint8_t dev_num);

/**
 * Check the device without an acknowledgment of a specific message, if they
 * are in range it sends a message to them. If there is no acknowledgment for a
 * message it removes it.
 *
 * @param pid PID of the device
 * @param dev_num Index of the device
 * @param position Current device position
 * @param list List of the messages to check
 */
void check_send_messages(pid_t pid, uint8_t dev_num, vec_2 position,
                         list_message *list);

/**
 * Read all the messages in the FIFO. It will exit on error. If there is no
 * message with the same message id it will not add it to the message list. If a
 * message is added it will write the acknowledgment on the shared memory.
 *
 * @param fifo The device FIFO.
 * @param dev_num Index of the device
 * @param list List of the device messages
 */
void recv_messages_device(int fifo, uint8_t dev_num, list_message *list);
