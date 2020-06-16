/// @file device.c
/// @brief Contains the implementation of function and variables for the device

#include "device.h"
#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include "semaphore.h"
#include "server_lib.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void device_process(size_t dev_num) {
  // get pid
  pid_t pid = getpid();
  // Device position
  vec_2 position = {0, 0};

  // make FIFO
  make_fifo_device(pid);
  // open FIFO device
  char *path = pid_fifo_path(pid);
  int fifo = open(path, O_RDONLY);
  if (fifo == -1)
    err_exit("open", __FILE__, __LINE__);
  free(path);

  while (1) {
    // With for moving
    semaphore_op(semid, dev_num, -1);

    // Check if acknowledgement and acknowledgement are blocked, then blocks
    // them
    semaphore_op(semid, 6, 0);
    semaphore_op(semid, 6, 1);
    semaphore_op(semid, 7, 0);
    semaphore_op(semid, 7, 1);

    bool msg_sent;
    // Chicle throw the acknowledgement if there is a message to be send
    for (uint8_t i = 0 + dev_num; i < ACK_SIZE; i += DEVICE_NUMBER) {
      if (shm_ack[i].timestamp != 0) {
        msg_sent = false;
        // Check the board for a device, get the device number and check if we
        // need to send a message to it and then send it
        for (uint8_t j = 0; j < BOARD_SIZE && !msg_sent; j++) {
          for (uint8_t k = 0; k < BOARD_SIZE && !msg_sent; k++) {
            if (shm_board[j * DEVICE_NUMBER + k] != 0) {
              // Device number
              uint8_t d_i = 0;
              while (pid_devices[d_i] != shm_board[j * DEVICE_NUMBER + k])
                d_i++;

              // Check if we need to send a message
              if (shm_ack[i - dev_num + d_i].timestamp == 0) {
                // check if the distance is inferior to max distance
                if (distance_sqr(position, (vec_2){i, j})) {
                  // Set msg_sent to pass to the next message
                  msg_sent = true;
                }
              }
            }
          }
        }
      }
    }
  }

  // read messages
}
