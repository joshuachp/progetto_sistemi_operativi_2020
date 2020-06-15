/// @file server_lib.h
/// @brief  Contiene la definizioni di variabili e funzioni specifiche per il
///         SERVER.

#pragma once

#include "defines.h"
#include "position.h"
#include <stdlib.h>
#include <sys/types.h>

// Board shared memory
extern int shmid_board;
extern pid_t *shm_board;
// Acknowledgment shared memory
extern int shmid_ack;
// Shared array size ACK_SIZE
extern Acknowledgment *shm_ack;
// Positions shared memory
extern int shmid_positions;
// Shared array size DEVICE_NUMBER
extern vec_2 *shm_positions;
// Semaphore set id
extern int semid;
// Message queue id
extern int msqid;
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
 * @param signum Signal that was blocked
 */
void termination_handler(int);

/**
 * Setup signal handling for the application
 */
void setup_sig_handler();

/**
 * Setup up the shared memories, semaphores and message queue
 *
 * @param key Message queue key
 */
void set_up_server(key_t key);

/**
 * Prints the pid, position and message of each process
 *
 * @param step The current step
 */
void print_status(size_t step);

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
 * @param dev_num Device number for semaphores
 */
void device_process(size_t dev_num);
