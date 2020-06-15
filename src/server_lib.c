/// @file server.c
/// @brief  Contiene l'implementazione delle funzioni del SERVER per permetterne
///         il testing.

#include "server_lib.h"
#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include "message.h"
#include "position.h"
#include "semaphore.h"
#include "shared_memory.h"
#include <fcntl.h>
#include <signal.h>
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
int shmid_positions;
vec_2 *shm_positions;
int semid;
int msqid;
pid_t pid_server;
pid_t pid_ack;
pid_t pid_devices[DEVICE_NUMBER];

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
        kill(pid_devices[i], SIGKILL);
      }

      // Remove FIFOs
      for (size_t i = 0; i < DEVICE_NUMBER; i++) {
        remove_fifo_device(pid_devices[i]);
      }

      // Remove board shared memory
      remove_shared_memory(shmid_board);

      // Remove acknowledgement shared memory
      remove_shared_memory(shmid_ack);

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

  // Create, attach and sets to NULL shared memory acknowledgement
  shmid_ack =
      alloc_shared_memory(IPC_PRIVATE, sizeof(Acknowledgment) * ACK_SIZE);
  shm_ack = get_shared_memory(shmid_ack, 0);
  shm_ack = memset(shm_ack, 0, sizeof(Acknowledgment) * ACK_SIZE);

  // Create and attach shared memory positions
  shmid_positions =
      alloc_shared_memory(IPC_PRIVATE, sizeof(vec_2) * DEVICE_NUMBER);
  shm_positions = get_shared_memory(shmid_positions, 0);

  // Create 7 semaphores and initialize it
  semid = semget(IPC_PRIVATE, 4,
                 IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP);
  if (semid == -1)
    err_exit("semget", __FILE__, __LINE__);
  // Initialize the semaphore set with semctl. The first 5 variables are for
  // the devices then board and the last one acknowledgement
  unsigned short semInitVal[] = {0, 0, 0, 0, 0, 0, 0};
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
  for (int i = 0; i < DEVICE_NUMBER; i++) {
    // TODO: Message list
    printf("%u %hhu %hhu msgs: lista message_id\n", pid_devices[i],
           shm_positions[i].i, shm_positions[i].j);
  }
  puts("#############################################");
}

void server_process(list_positions *list) {
  // Current positions
  node_positions *node = list->head;

  for (size_t i = 0; i < list->length; i++) {
    // Waits two seconds
    sleep(SLEEP_TIME_SERVER);

    // Set positions of all devices
    shm_positions =
        memcpy(shm_positions, node->value, sizeof(vec_2) * DEVICE_NUMBER);

    // Print positions
    print_status(i);

    // Unlock first device
    semaphore_op(semid, 0, 1);

    // Get next position
    node = node->next;
  }
}

void device_process(size_t dev_num) {
  // get pid
  pid_t pid = getpid();

  // Message list
  list_message *messages = create_list_message(NULL, NULL, 0);

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

    // send messages if any
    if (messages->length > 0) {
      semaphore_op(semid, dev_num, -1);
    }

    // read messages
  }
}
