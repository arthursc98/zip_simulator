/*
 * main.c
 * Implementing a simulation of a zip file
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

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include "sysfiles.h"
 #include "gui.h"

 int main() {
	int op;
	while(true){
		menu();
		scanf(" %d", &op);
		clearscr();
		switch (op)
		{
		case 1:
			createFiles();
			break;

		case 2:
			appendFiles();
			break;

		case 3:
			delete();
			break;

		case 4:
			listFiles();
			break;

		case 5:
			showFile();
			break;

		case 6:
			unpack();
			break;

		case 7:
			recover();
			break;

		case 0:
			cya();
			remove("sofilesystem.chk");
			remove("sofilesystem.sfs");
			return 0;

		default:
			error();
			printf("Invalid option, please use some option from the menu");
			pauseSys();
			clearscr();
			break;
		}
	}
 }