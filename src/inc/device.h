/// @file device.h
/// @brief Contains the declaration of function and variables for the device..

#pragma once

#include <sys/types.h>

/**
 * Function for the devices, create a FIFO
 *
 * @param dev_num Device number for semaphores
 */
void device_process(size_t dev_num);

