/*
 * helpers.h
 * Defining util functions for main operations
 *
 * Arthur Silveira Chaves (Sistemas de Informação)
 * Beatriz Jonas Justino (Sistemas de Informação)
 * Nicholas Meirinho Perez (Sistemas de Informação)
 * Rafael Oliveira(Sistemas de Informação)
 *
 * Class/Subject: Operating Systems
 *
 * 10/11/2020
 */

#ifndef _HELPERS_H
#define _HELPERS_H
#include <stdbool.h>

// Check if a file exists at current dir
bool fileExists(char * filename);
// CreateDirectory
void createDir();
#endif