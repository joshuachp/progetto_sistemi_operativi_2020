/// @file server.c
/// @brief  Contiene l'implementazione delle funzioni del SERVER per permetterne
///         il testing.

#include "server_lib.h"
#include "defines.h"
#include "err_exit.h"
#include "position.h"
#include "semaphore.h"
#include "shared_memory.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>

int shmid_board;
int shmid_ack;
int semid;

void print_help(void) {
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
      // Remove board shared memory
      remove_shared_memory(shmid_board);
      // Remove acknowledgement shared memory
      remove_shared_memory(shmid_ack);
      // Remove semaphore set
      if (semctl(semid, 0, IPC_RMID, 0) == -1)
        err_exit("semctl IPC_RMID");
      exit(0);
  }
}

void setup_sig_handler() {
  // Register signal handler for SIGTERM to cleanup and terminate the process
  // beautifully.
  // XXX: Using sigaction instead of signal, see "Portability"
  // https://linux.die.net/man/2/signal
  struct sigaction action;
  sigset_t block_mask;

  // Blocking signals
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGSTOP);

#ifdef NDEBUG
  // Block <C-c> for not debug build
  sigaddset(&block_mask, SIGINT);
#endif

  sigprocmask(SIG_BLOCK, &block_mask, NULL);

  // Setting up SIGTERM handler
  action.sa_handler = termination_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  if (sigaction(SIGTERM, &action, NULL) == -1)
    err_exit("sigaction");

#ifndef NDEBUG
  // Permit <C-c> for debug build
  if (sigaction(SIGINT, &action, NULL) == -1)
    err_exit("sigaction");
#endif
}

void set_up_server() {
  // Create shared memory board, create a 10*10 int data segment that will be
  // accessible throw pointer arithmetics
  shmid_board =
      alloc_shared_memory(IPC_PRIVATE, sizeof(pid_t) * BOARD_SIZE * BOARD_SIZE);
  // Create shared memory acknowledgement
  shmid_ack =
      alloc_shared_memory(IPC_PRIVATE, sizeof(Acknowledgment) * DEVICE_NUMBER);
  // Create 7 semaphores and initialize it
  semid = semget(IPC_PRIVATE, 4, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP);
  if (semid == -1)
    err_exit("semget");
  // Initialize the semaphore set with semctl
  // TODO: Check sem values
  unsigned short semInitVal[] = {0, 0, 0, 0, 0, 0, 0};
  union semun arg;
  arg.array = semInitVal;
  if (semctl(semid, 0, SETALL, arg) == -1)
    err_exit("semctl SETALL");
}

void print_status(size_t step, pid_t devices[], node_positions *positions) {
  printf("# Step %zu: device positions ########################\n", step);
  for (int i = 0; i < DEVICE_NUMBER; i++) {
    // TODO: Message list
    printf("%u %hhu %hhu msgs: lista message_id\n", devices[i],
           positions->value[i].i, positions->value[i].j);
  }
  puts("#############################################");
}

void server_process(list_positions *positions) {
  // Waits two seconds
  sleep(SLEEP_TIME);
}
