/*
 * helpers.c
 * Implementing util functions for main operations.
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
#include <stdbool.h>
#include <sys/stat.h>
#include "helpers.h"

// region [Helpers]
bool fileExists(char * filename) {
	struct stat buffer;
	return (stat (filename, &buffer) == 0);
}

void createDir() {
	#if defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
		mkdir("./unpacked_files", 0700);
	#elif _WIN32
		_mkdir("./unpacked_files");
	#endif
}
// endregion [Helpers]