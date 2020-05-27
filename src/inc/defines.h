/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include <time.h>
#include <unistd.h>

#define BUFF_READ_SIZE 256

/**
 * Messages sent from the clients to the devices
 */
typedef struct {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  char message[256];
  double max_distance;
} message_ds;

/**
 * Acknowledgement saved in the shared memory
 * TODO: Not sure if this is needed in the client
 */
typedef struct {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  time_t timestamp;
} acknowledgment_ds;
