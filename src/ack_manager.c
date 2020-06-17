/// @file   ack_manager.c
/// @brief  Contains the declaration of functions and variables for the SERVER.

#include "ack_manager.h"
#include "defines.h"
#include "err_exit.h"
#include "semaphore.h"
#include "server_lib.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/msg.h>

void ack_manager_process() {

  bool ack_check;

  while (1) {
    sleep(SLEEP_TIME_ACK_MANAGER);

    // Block the acknowledgment shared memory
    semaphore_op(semid, ACK_SEM, -1);

    // Cycle throw message ids
    for (uint8_t msg_id = 0; msg_id < ACK_SIZE; msg_id++) {

      // Check the acknowledgment for every device
      ack_check = true;

      for (uint8_t dev_num = 0; dev_num < DEVICE_NUMBER && ack_check;
           dev_num++) {
        // Check the message_id
        if (shm_ack(msg_id, dev_num).timestamp == 0) {
          ack_check = false;
        }
      }

      // We can remove the acknowledgment and send the message to the client
      if (ack_check) {

        // Create the message
        ack_msg *ack = malloc(sizeof(ack_msg));

        // Sets the message type to the message id to filter messages
        // XXX: Plus 1 to filter id 0
        ack->mtype = msg_id + 1;
        // Copy the acknowledgment list
        memcpy(ack->ack_list, &shm_ack(msg_id, 0),
               sizeof(Acknowledgment) * DEVICE_NUMBER);
        // Send message to client
        if (msgsnd(msqid, ack, sizeof(ack->ack_list), 0) == -1)
          err_exit("msgsnd", __FILE__, __LINE__);

        // Free message and resets acknowledgments to 0
        free(ack);
        memset(&shm_ack(msg_id, 0), 0, sizeof(Acknowledgment) * DEVICE_NUMBER);
      }
    }

    // Unlock acknowledgment shared memory
    semaphore_op(semid, ACK_SEM, 1);
  }
}
