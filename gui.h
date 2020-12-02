
/*
 * gui.h
 * Declaring functions for GUI.
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

#ifndef _GUI_H
#define _GUI_H
#include <stdio.h>

// Create a mini menu
void menu();
// Pauses the current application
void pauseSys();
// Clearscreen
void clearscr();
// Claim error
void error();
// Exit program
void cya();
// Print Line
void line(int size);
// Log Error
void logError(char * str);
// Print Input Filename
void printInputFilename();
// Print Input Recover Filename
void printInputRecoverFilename();
// Print Input Delete Filename
void printInputDeleteFilename();
// Print Input Filename Content Header
void printInputFilenameContent();
// Print Input Origin Path
void printInputOriginPath();
// Print File Doesnt Exists
void printFileDoesntExists();
// Print Deleted File
void printDeletedFile();
// Print Unpacked Files
void printUnpackedFiles();
#endif