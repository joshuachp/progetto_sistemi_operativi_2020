/// @file err_exit.h
/// @brief Contiene la definizione della funzione di stampa degli errori.

#pragma once

/*
 * Prints the error message of the last failed system call and terminates
 * the calling process.
 */
void err_exit(const char *msg);
