/*
 * sysfiles.h
 * Defining main functions of simulation
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

#ifndef _SYSFILES_H
#define _SYSFILES_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct fileInfo {
	char filename[256];
	char originPath[256];
	char directory[256];
	char * type;
	int initialByte;
	int bytesSize;
	int modifiedDate;
	int creationDate;
	bool deleted;
};

struct headerInfo {
	int version;
	int createDate;
	int modifiedDate;
	int nFiles;
	int headerSize;
};

typedef struct fileInfo * file;
typedef struct headerInfo * header;

// Create a base file for simulation
void createFiles();

// Append a new file
void appendFiles();

// Delete file from simulation
void delete();

// List files inside zip
void listFiles();

// Show content file
void showFile();

// Unpack files
void unpack();

// Recover file
void recover();

// Get file name from stdin 
char * inputFilename();
char * inputRecoverFilename();
char * inputDeleteFilename();
char * inputFilenameReadContent();

// Get file paths from stdin
char * inputOriginPath();

// Get extension from file
char * getExtension(file * newFile);

// Get file size
int getFileSize(file * newFile);

// Get creation and last modified date from file
void getDates(file * newFile, int * modifiedDate, int * creationDate);

// Check if file already exists on .sfs
void checkExistingFile(file newFile, bool * exists, bool * recoverable);

#endif