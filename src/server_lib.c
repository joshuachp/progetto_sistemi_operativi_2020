/// @file server.c
/// @brief  Contiene l'implementazione delle funzioni del SERVER per permetterne
///         il testing.

#include "server_lib.h"
#include "err_exit.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

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
