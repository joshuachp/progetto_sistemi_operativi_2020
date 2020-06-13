/// @file err_exit.h
/// @brief Contiene la definizione della funzione di stampa degli errori.

#pragma once

/*
 * Prints the error message of the last failed system call and terminates
 * the calling process, with the file and line it generated from, and exits the
 * process with status 1.
 *
 * @param msg Error message
 * @param file File the error generated on
 * @param line Line the error generated on
 */
void err_exit(const char *msg, char *file, int line);

/*
 * Prints an error with the file and line that generated it
 *
 * @param msg Error message
 * @param file File the error generated on
 * @param line Line the error generated on
 */
void print_err(const char *msg, char *file, int line);
/*
 * Prints the error message of the last failed system call and terminates
 * the calling process, with the file and line it generated from.
 *
 * @param msg Error message
 * @param file File the error generated on
 * @param line Line the error generated on
 */
void print_perror(const char *msg, char *file, int line);
