/// @file server.c
/// @brief Contiene l'implementazione del SERVER.

#include "defines.h"
#include "err_exit.h"
#include "fifo.h"
#include "files.h"
#include "position.h"
#include "semaphore.h"
#include "shared_memory.h"
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

int main(int argc, char *argv[]) {

  uint32_t key;
  if (argc != 3 || sscanf(argv[1], "%u", &key) != 1) {
    print_help();
    return 1;
  }
  char *file = argv[2];
  // XXX: Testing
  list_positions *positions = read_positions_file(file);

  // Signals setup
  setup_sig_handler();

  return 0;
}

void print_help(void) {
  puts("Usage:\n"
       "  server KEY FILE\n"
       "\n"
       "With the KEY of the message queue and FILE for the positions.");
}

void termination_handler(int signum) {
  // TODO: Terminate beautifully
  exit(0);
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

