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
#define ACK_SIZE 100
#define SLEEP_TIME_SERVER 2

/**
 * Acknowledgement saved in the shared memory
 */
typedef struct Acknowledgement {
  pid_t pid_sender;
  pid_t pid_receiver;
  int message_id;
  time_t timestamp;
} Acknowledgment;

/**
 * Acknowledgement sent to the client
 */
typedef struct ack_msg {
  long mtype;
  Acknowledgment ack_list[5];
} ack_msg;
