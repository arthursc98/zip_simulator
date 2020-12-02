/*
 * gui.c
 * Implementing functions for GUI.
 *
 * Arthur Silveira Chaves (Sistemas de Informação)
 * Beatriz Jonas Justino (Sistemas de Informação)
 * Nicholas Meirinho Perez (Sistemas de Informação)
 * Rafael Oliveira (Sistemas de Informação)
 *
 * Class/Subject: Operating Systems
 *
 * 10/11/2020
 */

#include "gui.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

// region [GUI]

void menu() {
	fprintf(stdout, "+=============================================================+\n");
	fprintf(stdout, "|              Catholic University of Santos                  |\n");
	fprintf(stdout, "|                                                             |\n");
	fprintf(stdout, "|                     University Assigment                    |\n");
	fprintf(stdout, "|              Class/Subject: Operating Systems               |\n");
	fprintf(stdout, "|                        Semester: 6                          |\n");
	fprintf(stdout, "|                                                             |\n");
	fprintf(stdout, "|                        Developed by:                        |\n");
	fprintf(stdout, "|                  Arthur Silveira Chaves                     |\n");
	fprintf(stdout, "|                   Beatriz Jonas Justino                     |\n");
	fprintf(stdout, "|                  Nicholas Meirinho Perez                    |\n");
	fprintf(stdout, "|                                                             |\n");
	fprintf(stdout, "+=============================================================+\n");
	fprintf(stdout, "|               1-Create simulation file                      |\n");
	fprintf(stdout, "|               2-Append new file to simulation               |\n");
	fprintf(stdout, "|               3-Delete file from simulation                 |\n");
	fprintf(stdout, "|               4-List files inside simulaiton                |\n");
	fprintf(stdout, "|               5-Show file content                           |\n");
	fprintf(stdout, "|               6-Unpack Files                                |\n");
	fprintf(stdout, "|               7-Recover File                                |\n");
	fprintf(stdout, "|               0-Exit                                        |\n");
	fprintf(stdout, "+=============================================================+\n");
}

void clearscr() {
	#ifdef _WIN32
		system("cls");
	#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
		system("clear");
	#else
		#error "OS not supported."
	#endif
}

void pauseSys() {
	fprintf(stdout, "Press any key to continue...");
	getchar();
	scanf("c\n");
}

void error() {
	fprintf(stdout, "+==================================+\n");
	fprintf(stdout, "|               ERROR              |\n");
	fprintf(stdout, "+==================================+\n");
}


void cya() {
	fprintf(stdout, "+==================================+\n");
	fprintf(stdout, "|           See you later          |\n");
	fprintf(stdout, "+==================================+\n");
}

void line(int size)
{
    fprintf(stdout, "+");
    size-=2;
    while(size--)
        fprintf(stdout, "=");
    fprintf(stdout, "+\n");
}

void logError(char * str)
{
    int size = strlen(str);
    line(size + 4);
    fprintf(stderr, "| %s |\n", str);
    line(size + 4);
}
void printUnpackedFiles() {
	fprintf(stdout, "+=======================================================================================+\n");
	fprintf(stdout, "| Your files has been exported to a dir called: unpacked_files at the current directory |\n");
	fprintf(stdout, "+=======================================================================================+\n");
}

void printDeletedFile() {
	fprintf(stdout, "+====================================================+\n");
	fprintf(stdout, "|          Successfully deleted requested file!      |\n");
	fprintf(stdout, "+====================================================+\n");
}

void printInputFilename() {
	fprintf(stdout, "+====================================================+\n");
	fprintf(stdout, "|                   Enter the file name              |\n");
	fprintf(stdout, "|                   e.g.  examples.txt               |\n");
	fprintf(stdout, "+====================================================+\n>>>> ");
}

void printInputRecoverFilename() {
	fprintf(stdout, "+====================================================+\n");
	fprintf(stdout, "|           Enter the file you want to recover       |\n");
	fprintf(stdout, "|                   e.g.  examples.txt               |\n");
	fprintf(stdout, "+====================================================+\n>>>> ");
}

void printInputDeleteFilename() {
	fprintf(stdout, "+====================================================+\n");
	fprintf(stdout, "|        Enter the file name that will be deleted    |\n");
	fprintf(stdout, "|                   e.g.  examples.txt               |\n");
	fprintf(stdout, "+====================================================+\n>>>> ");
}

void printInputFilenameContent() {
	fprintf(stdout, "+====================================================+\n");
	fprintf(stdout, "|                   Enter the file name              |\n");
	fprintf(stdout, "|                   e.g.  examples.txt               |\n");
	fprintf(stdout, "|                Available extensions: txt           |\n");
	fprintf(stdout, "+====================================================+\n>>>> ");
}

void printInputOriginPath(){
	fprintf(stdout, "+=========================================================+\n");
	fprintf(stdout, "|                    Enter the file path                  |\n");
	fprintf(stdout, "| Note: If your using a unix system you can use path like:|\n");
	fprintf(stdout, "| ./dir/someotherdir/                                     |\n");	
	fprintf(stdout, "| If your using a Windows system try like:                |\n");	
	fprintf(stdout, "| c:\\\\ABC.txt                                             |\n");	
	fprintf(stdout, "+=========================================================+\n>>>> ");
}

void printFileDoesntExists(){
	fprintf(stdout, "+===========================================================+\n");
	fprintf(stdout, "| The requested file doesn't exists on your compressed file |\n");
	fprintf(stdout, "+===========================================================+\n");
}
// endregion [GUI]