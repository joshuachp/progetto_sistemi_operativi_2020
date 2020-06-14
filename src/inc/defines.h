/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define BUF_READ_SIZE 256
#define MIN_POSITIONS_ARRAY_LENGTH 6
#define BOARD_SIZE 10
#define DEVICE_NUMBER 5
#define SLEEP_TIME_SERVER 2

/**
 * Messages sent from the clients to the devices
 */
typedef struct {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  char message[256];
  // XXX: Changed from double because we use the squared values to check the
  // distances, so the max_distance (squared) max value can only be
  // 10^2+10^2=200, that is approximately 15 squared.
  uint8_t max_distance;
} Message;

/**
 * Acknowledgement saved in the shared memory
 */
typedef struct {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  time_t timestamp;
} Acknowledgment;

/**
 * Acknowledgement sent to the client
 */
typedef struct {
  long mtype;
  Acknowledgment ack;
} ack_msg;
