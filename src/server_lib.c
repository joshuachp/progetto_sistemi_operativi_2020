/// @file server.c
/// @brief  Contiene l'implementazione delle funzioni del SERVER per permetterne
///         il testing.

#include "server_lib.h"
#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include "position.h"
#include "semaphore.h"
#include "shared_memory.h"
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

int shmid_board;
pid_t *shm_board;
int shmid_ack;
Acknowledgment *shm_ack;
int shmid_dev;
pid_t *shm_dev;
int shmid_positions;
vec_2 *shm_positions;
int semid;
int msqid;
pid_t pid_server;
pid_t pid_ack;

void print_help_server(void) {
  puts("Usage:\n"
       "  server KEY FILE\n"
       "\n"
       "With the KEY of the message queue and FILE for the positions.");
}

void termination_handler(int signum) {
  // XXX: Remove warning of unused signum
  switch (signum) {
    case SIGTERM:
#ifndef NDEBUG
    case SIGINT:
#endif
      // Kill processes
      kill(pid_ack, SIGKILL);
      for (size_t i = 0; i < DEVICE_NUMBER; i++) {
        kill(shm_dev[i], SIGKILL);
      }

      // Remove FIFOs
      for (size_t i = 0; i < DEVICE_NUMBER; i++) {
        remove_fifo_device(shm_dev[i]);
      }

      // Remove board shared memory
      remove_shared_memory(shmid_board);

      // Remove acknowledgement shared memory
      remove_shared_memory(shmid_ack);

      // Remove device PID shared memory
      remove_shared_memory(shmid_dev);

      // Remove positions shared memory
      remove_shared_memory(shmid_positions);

      // Remove semaphore set
      if (semctl(semid, 0, IPC_RMID, 0) == -1)
        print_perror("semctl", __FILE__, __LINE__);

      // Remove message queue
      if (msgctl(msqid, IPC_RMID, 0) == -1)
        print_perror("msgctl", __FILE__, __LINE__);

      exit(0);
  }
}

void setup_sig_handler() {
  // Register signal handler for SIGTERM to cleanup and terminate the process
  // beautifully.
  // NOTE: Using sigaction instead of signal, see "Portability"
  // https://linux.die.net/man/2/signal
  struct sigaction action;
  sigset_t block_mask;

  // Blocking signals
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGSTOP);

#ifdef NDEBUG
  // Block <C-c> for release build
  sigaddset(&block_mask, SIGINT);
#endif

  sigprocmask(SIG_BLOCK, &block_mask, NULL);

  // Setting up SIGTERM handler
  action.sa_handler = termination_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  if (sigaction(SIGTERM, &action, NULL) == -1)
    err_exit("sigaction", __FILE__, __LINE__);

#ifndef NDEBUG
  // Permit <C-c> for debug build
  if (sigaction(SIGINT, &action, NULL) == -1)
    err_exit("sigaction", __FILE__, __LINE__);
#endif
}

void set_up_server(key_t key) {
  pid_server = getpid();

  // Create and attach shared memory board, create a 10*10 int data segment that
  // will be accessible throw pointer arithmetics
  shmid_board =
      alloc_shared_memory(IPC_PRIVATE, sizeof(pid_t) * BOARD_SIZE * BOARD_SIZE);
  shm_board = get_shared_memory(shmid_board, 0);
  shm_board = memset(shm_board, 0, sizeof(pid_t) * BOARD_SIZE * BOARD_SIZE);

  // Create, attach and sets to NULL shared memory acknowledgement
  shmid_ack = alloc_shared_memory(IPC_PRIVATE, sizeof(Acknowledgment) *
                                                   ACK_SIZE * DEVICE_NUMBER);
  shm_ack = get_shared_memory(shmid_ack, 0);
  shm_ack = memset(shm_ack, 0, sizeof(Acknowledgment) * ACK_SIZE);

  // Create and attach shared memory positions
  shmid_dev = alloc_shared_memory(IPC_PRIVATE, sizeof(pid_t) * DEVICE_NUMBER);
  shm_dev = get_shared_memory(shmid_dev, 0);

  // Create and attach shared memory positions
  shmid_positions =
      alloc_shared_memory(IPC_PRIVATE, sizeof(vec_2) * DEVICE_NUMBER);
  shm_positions = get_shared_memory(shmid_positions, 0);
  shm_positions = memset(shm_positions, 0, sizeof(vec_2) * DEVICE_NUMBER);

  // Create 7 semaphores and initialize it
  semid = semget(IPC_PRIVATE, 7,
                 IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP);
  if (semid == -1)
    err_exit("semget", __FILE__, __LINE__);
  // Initialize the semaphore set with semctl. The first 5 variables are for
  // the devices then board and the last one acknowledgement
  unsigned short semInitVal[] = {0, 0, 0, 0, 0, 1, 1};
  union semun arg;
  arg.array = semInitVal;
  if (semctl(semid, 0, SETALL, arg) == -1)
    err_exit("semctl", __FILE__, __LINE__);

  // Create message queue
  msqid = msgget(key, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP);
  if (msqid == -1)
    err_exit("msgget", __FILE__, __LINE__);
}

void print_status(size_t step) {
  printf("# Step %zu: device positions ########################\n", step);
  for (uint8_t dev_num = 0; dev_num < DEVICE_NUMBER; dev_num++) {
    printf("%u %hhu %hhu msgs:", shm_dev[dev_num], shm_positions[dev_num].i,
           shm_positions[dev_num].j);

    // Cycle throw acknowledgements to print message list for each device
    bool first = true;
    for (uint8_t msg_id = 0; msg_id < ACK_SIZE; msg_id++) {
      if (shm_ack(msg_id, dev_num).timestamp != 0) {
        if (first) {
          first = false;
          printf(" %u", msg_id);
        } else {
          printf(", %u", msg_id);
        }
      }
    }
    if (first) {
      printf(" <empty>\n");
    } else {
      printf("\n");
    }
  }
  puts("###################################################\n");
  fflush(stdout);
}

void server_process(list_positions *list) {
  // Current positions
  node_positions *node;
  while (1) {
    node = list->head;

    for (size_t i = 0; i < list->length; i++) {
      // Waits two seconds
      sleep(SLEEP_TIME_SERVER);

      // Print positions
      print_status(i);

      // Set positions of all devices
      shm_positions =
          memcpy(shm_positions, node->value, sizeof(vec_2) * DEVICE_NUMBER);

      // Unlock first device
      semaphore_op(semid, 0, 1);

      // Get next position
      node = node->next;
    }
  }
}

