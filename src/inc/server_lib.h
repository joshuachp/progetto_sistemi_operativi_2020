/// @file server_lib.h
/// @brief  Contiene la definizioni di variabili e funzioni specifiche per il
///         SERVER.

#pragma once

// Board shared memory id
int shmid_board;
// Acknowledgment shared memory id
int shmid_ack;
// Semaphore set id
int semid;

/*
 * Prints the help for the binary
 */
void print_help(void);

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
void setUpServer();
