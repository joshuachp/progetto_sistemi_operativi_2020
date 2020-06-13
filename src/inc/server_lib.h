/// @file server_lib.h
/// @brief  Contiene la definizioni di variabili e funzioni specifiche per il
///         SERVER.

#pragma once

#include "position.h"
#include <stdlib.h>

// Board shared memory id
extern int shmid_board;
// Acknowledgment shared memory id
extern int shmid_ack;
// Semaphore set id
extern int semid;
// PID of the acknowledgement manager
extern pid_t pid_server;
// PID of the acknowledgement manager
extern pid_t pid_ack;
// PID of each device
extern pid_t pid_devices[5];

/*
 * Prints the help for the SERVER
 */
void print_help_server(void);

/**
 * Handles the closing the all the forked processes and shared memories
 * segments before closing the server process beautifully.
 *
 * @param signum SIGTERM
 */
void termination_handler(int);

/**
 * Setup signal handling for the application
 */
void setup_sig_handler();

/**
 * Setup up the shared memories and semaphores
 */
void set_up_server();

/**
 * Prints the pid, position and message of each process
 *
 * @param step The current step
 * @param devices Array of device pids
 * @param positions Current node of devices positions
 */
void print_status(size_t step, pid_t devices[], node_positions *positions);

/**
 * Function for the server, every two seconds prints the positions and move the
 * devices.
 *
 * @param list Lista delle posizioni
 */
void server_process(list_positions *list);

/**
 * Function for the devices, create a FIFO
 *
 */
void device_process();
