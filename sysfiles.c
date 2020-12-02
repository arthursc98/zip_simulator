/*
 * sysfiles.c
 * Implementing main functions of simulation
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
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "gui.h"
#include "helpers.h"
#include "sysfiles.h"

// region [Main Operations]
void createFiles() {
	char * files [2][1] = {
		{"sofilesystem.chk"},
		{"sofilesystem.sfs"},
	};

	for(int i = 0; i < 2; i++){
		if (!fileExists(files[i][0])){
			FILE * f;
			if((f = fopen(files[i][0], "w+b")) == NULL) {
				char error[100] = "Erro: Won't be able to create the file";
				strcat(error, files[i][0]);
				logError(error);
			} else {
				if (strcasecmp("sofilesystem.chk", files[i][0]) == 0) {
					header h;
					if ((h = (header) malloc(sizeof(struct headerInfo))) == NULL) {
						fprintf(stderr, "Unable to alloc memory for header!\n");
						exit(1);
					}
					fseek(f, 0, SEEK_SET);
					fprintf(stdout, "Version\n>>> ");
					fscanf(stdin, " %d", &h->version);
					if (h->version < 10000000){
						time_t now;
						time(&now);

						struct tm* now_tm;
						now_tm = localtime(&now);
						char today[12];
						strftime(today, 12, "%Y%m%d", now_tm);

						h->createDate = atoi(today);
						h->modifiedDate = h->createDate;
						h->nFiles = 0;
						h->headerSize = sizeof(h);

						fwrite(&h, sizeof(header), 1, f);
						free(h);
						clearscr();
					} else {
						char error[100] = "Erro: Invalid version";
						logError(error);
					}
				}
			}
			fclose(f);
		} else {
			fprintf(stdout, "The S.O ");
			fprintf(stdout, "%s ", files[i][0]);
			fprintf(stdout, "already exists\n");
			pauseSys();
			clearscr();
		}
	}
}

void appendFiles() {
	file newFile;
	header headerMod;
	int modifiedDate, creationDate;
	bool exists, recoverable;
	if ((newFile = (file) malloc(sizeof(struct fileInfo))) == NULL){
		fprintf(stderr, "Unable to alloc memory for new file!\n");
		exit(1);
	}
	if ((headerMod = (header) malloc(sizeof(struct headerInfo))) == NULL){
		fprintf(stderr, "Unable to alloc memory for modified header!\n");
		exit(1);
	}
	FILE * sfs, * currentFile, * chk;
	if((chk = fopen("sofilesystem.chk", "r+b")) == NULL) {
		char error[100] = "Erro: Won't be able to create the file";
		strcat(error, "sofilesystem.chk");
		logError(error);
	}
	if((sfs = fopen("sofilesystem.sfs", "a+b")) == NULL) {
		char error[100] = "Erro: Won't be able to create the file";
		strcat(error, "sofilesystem.sfs");
		logError(error);
	} else {
		// region [Adding new file]
		char * aux = inputFilename();
		strcpy(newFile->filename, aux);

		aux = inputOriginPath();
		strcpy(newFile->originPath, aux);

		checkExistingFile(newFile, &exists, &recoverable);
		if(exists){
			if(recoverable){
				fprintf(stdout, "+=================================================================================================+\n");
				fprintf(stdout, "+ Hey, you already add this file, if you want to recover it, go to the menu and choose the option +\n");
				fprintf(stdout, "+=================================================================================================+\n");
			} else {
				fprintf(stdout, "+===========================================================+\n");
				fprintf(stdout, "| The requested file already exists on your compressed file |\n");
				fprintf(stdout, "+===========================================================+\n");
			}
			pauseSys();
			clearscr();
			fclose(sfs);
		} else {
			newFile->type = getExtension(&newFile);
			newFile->bytesSize = getFileSize(&newFile);
			getDates(&newFile, &modifiedDate, &creationDate);
			newFile->modifiedDate = modifiedDate;
			newFile->creationDate = creationDate;
			newFile->deleted = false;
	  		free(aux);

			char * filePath = (char *)malloc(512 * sizeof(char));
			strcpy(filePath, newFile->originPath);
			strcat(filePath, newFile->filename);
			currentFile = fopen(filePath, "rb");
			free(filePath);
			fseek(currentFile, 0, SEEK_END);
			long filelen = ftell(currentFile);
			fseek(currentFile, 0, SEEK_SET);

			char * buffer = (char *)malloc(filelen * sizeof(char));
			fread(buffer, filelen, 1 , currentFile);
			fwrite(buffer, sizeof(char), filelen, sfs);
			free(buffer);

			unsigned char c[1];
			fseek(currentFile, 0, SEEK_SET);
			fread(&c, sizeof(char), 1, currentFile);
			newFile->initialByte = c[0];

			fclose(currentFile);

			fseek(chk, 0, SEEK_END);
			fwrite(&newFile, sizeof(file), 1, chk);
			// endregion [Adding new file]

			// region [Updating header]
			fseek(chk, 0, SEEK_SET);
			fread(&headerMod, sizeof(header), 1, chk);

			time_t now;
			time(&now);

			struct tm* now_tm;
			now_tm = localtime(&now);
			char today[12];
			strftime(today, 12, "%Y%m%d", now_tm);

			headerMod->modifiedDate = atoi(today);
			headerMod->nFiles = headerMod->nFiles + 1;
			headerMod->headerSize = sizeof(headerMod);

			fseek(chk, 0, SEEK_SET);
			fwrite(&headerMod, sizeof(header), 1, chk);
			// endregion [Updating header]

			clearscr();
			fclose(sfs);
		}
	}
	fclose(chk);
}

void delete(){
	FILE * chk, * sfs;
	bool exist = false;
	if((chk = fopen("sofilesystem.chk", "r+b")) == NULL) {
		char error[100] = "Erro: Won't be able to create the file";
		strcat(error, "sofilesystem.chk");
		logError(error);
	} else {
		fseek(chk, 0, SEEK_END);
		if (ftell(chk) != 0){
			int skipBytes = 0, fileBytesSize;
			char * filename = inputDeleteFilename();
			fseek(chk, 0, SEEK_SET);
			header headerReader = (header) malloc(sizeof(struct headerInfo));
			fread(&headerReader, sizeof(header), 1, chk);
			file fileRead[headerReader->nFiles];
			fread(&fileRead, sizeof(file), headerReader->nFiles, chk);
			for (int i = 0; i < headerReader->nFiles; i++){
				if(strcasecmp(fileRead[i]->filename, filename) == 0){
					exist = true;
					fileRead[i]->deleted = true;
					fileBytesSize = fileRead[i]->bytesSize;
					break;
				} else {
					skipBytes = skipBytes + fileRead[i]->bytesSize;
				}
			}
			if(!exist) {
				printFileDoesntExists();
			} else {
				fseek(chk, 0, SEEK_SET);
				fread(&headerReader, sizeof(header), 1, chk);
				fwrite(&fileRead, sizeof(file), headerReader->nFiles, chk);

				if((sfs = fopen("sofilesystem.sfs", "r+b")) == NULL) {
					char error[100] = "Erro: Won't be able to create the file";
					strcat(error, "sofilesystem.sfs");
					logError(error);
				} else {
					fseek(sfs, skipBytes, SEEK_SET);
					for(int i = 0; i < fileBytesSize; i++){
						fputs(" ", sfs);
					}
					fclose(sfs);
					clearscr();
					printDeletedFile();
				}
				fclose(chk);
			}
		} else {
			fprintf(stdout, "Your simulation file is empty. Append some files on it\n");
		}
	}
	pauseSys();
	clearscr();
}

void listFiles() {
	FILE * f;
	if((f = fopen("sofilesystem.chk", "r+b")) == NULL) {
		char error[100] = "Erro: Won't be able to read the file";
		strcat(error, "sofilesystem.chk");
		logError(error);
	} else {
		fseek(f, 0 , SEEK_END);
		if (ftell(f) != 0){
			fseek(f, 0, SEEK_SET);
			header headerReader = (header) malloc(sizeof(struct headerInfo));
			fread(&headerReader, sizeof(header), 1, f);
			file fileRead[headerReader->nFiles];
			fread(&fileRead, sizeof(file), headerReader->nFiles, f);
			for (int i = 0; i < headerReader->nFiles; i++){
				if (!fileRead[i]->deleted){
					fprintf(stdout, "+==========================================+\n");
					fprintf(stdout, "Filename: %s\n", fileRead[i]->filename);
					fprintf(stdout, "Origin Path: %s\n", fileRead[i]->originPath);
					fprintf(stdout, "Directory: %s\n", fileRead[i]->originPath);
					fprintf(stdout, "Creation Date: %d\n", fileRead[i]->creationDate);
					fprintf(stdout, "Modified Date: %d\n", fileRead[i]->modifiedDate);
					fprintf(stdout, "Initial Bytes: %08x\n", fileRead[i]->initialByte);
					fprintf(stdout, "File Bytes Size: %d\n", fileRead[i]->bytesSize);
					fprintf(stdout, "+==========================================+\n");
				}
			}
			pauseSys();
		}
	}
	clearscr();
	fclose(f);
}

void showFile() {
	file fileRead;
	if ((fileRead = (file) malloc(sizeof(struct fileInfo))) == NULL){
		fprintf(stderr, "Unable to alloc memory for file struct!\n");
		exit(1);
	}
	bool exists = false;
	char * externalFilename = inputFilenameReadContent();
	strcpy(fileRead->filename, externalFilename);
	char * extension = getExtension(&fileRead);
	if (strcasecmp(extension, "txt") == 0){
		FILE * f, * textFile;
		if((f = fopen("sofilesystem.chk", "rb")) == NULL) {
			char error[100] = "Erro: Won't be able to read the file";
			strcat(error, "sofilesystem.chk");
			logError(error);
		} else {
			fseek(f, 0, SEEK_END);
			int nFiles = (ftell(f) / sizeof(file));
			file fileRead[nFiles];
			fseek(f, 0, SEEK_SET);
			fread(&fileRead, sizeof(file), nFiles, f);
			for (int i = 0; i < nFiles; i++){
				if (!fileRead[i]->deleted){
					if (strcasecmp(externalFilename, fileRead[i]->filename) == 0) {
						exists = true;
						char * fullfile = malloc(512);
						strcpy(fullfile, fileRead[i]->originPath);
						strcat(fullfile, fileRead[i]->filename);
						if((textFile = fopen(fullfile, "r")) == NULL) {
							char error[100] = "Erro: Won't be able to read the file ";
							strcat(error, fullfile);
							logError(error);
						} else {
							clearscr();
							fprintf(stdout, "+==============+\n");
							fprintf(stdout, "| File Content |\n");
							fprintf(stdout, "+==============+\n");
							char c;
							while ((c = getc(textFile)) != EOF){
								putchar(c);
							}
							fclose(textFile);
						}
						break;
					}
				}
			}
			if(!exists){
				printFileDoesntExists();
			}
		}
		fclose(f);
	}
	else {
		fprintf(stdout, "+======================================+\n");
		fprintf(stdout, "| The requested file isn't a text file |\n");
		fprintf(stdout, "+======================================+\n");
	}
	free(externalFilename);
	free(fileRead);
	printf("\n");
	pauseSys();
	clearscr();
}

void unpack() {
	FILE * f, * unpack_file, * sfs;
	if((f = fopen("sofilesystem.chk", "r+b")) == NULL) {
		char error[100] = "Erro: Won't be able to read the file";
		strcat(error, "sofilesystem.chk");
		logError(error);
	} else {
		fseek(f, 0 , SEEK_END);
		if (ftell(f) != 0){
			fseek(f, 0, SEEK_SET);
			header headerReader = (header) malloc(sizeof(struct headerInfo));
			fread(&headerReader, sizeof(header), 1, f);
			file fileRead[headerReader->nFiles];
			fread(&fileRead, sizeof(file), headerReader->nFiles, f);

			struct stat st = {0};
			if (stat("./unpacked_files", &st) == -1) {
				createDir();
			}
			char fullpath[300];
			int skipBytes = 0;
			for (int i = 0; i < headerReader->nFiles; i++) {
				if(!fileRead[i]->deleted) {
					strcpy(fullpath, "./unpacked_files/");
					strcat(fullpath, fileRead[i]->filename);
					if((unpack_file = fopen(fullpath, "w+b")) == NULL) {
						char error[100] = "Erro: Won't be able to read the file";
						strcat(error, fullpath);
						logError(error);
					} else {
						if((sfs = fopen("sofilesystem.sfs", "r+b")) == NULL) {
							char error[100] = "Erro: Won't be able to read the file";
							strcat(error, "sofilesystem.sfs");
							logError(error);
						} else {
							if (i != 0){
								skipBytes = 0;
								for(int j = 0; j < i; j++){
									skipBytes = skipBytes + fileRead[j]->bytesSize;
								}
								fseek(sfs, skipBytes, SEEK_SET);
							}
							char * buffer = (char *)malloc(fileRead[i]->bytesSize * sizeof(char));
							fread(buffer, fileRead[i]->bytesSize, 1 , sfs);
							fwrite(buffer, sizeof(char), fileRead[i]->bytesSize, unpack_file);
							free(buffer);
						}
						fclose(sfs);
					}
					fclose(unpack_file);
				}
			}
			printUnpackedFiles();
		}
		else {
			fprintf(stdout, "+=================================================================+\n");
			fprintf(stdout, "| The currently file can't be unpacked, there's nothing to unpack |\n");
			fprintf(stdout, "+=================================================================+\n");
		}
		pauseSys();
		clearscr();
		fclose(f);
	}
}

void recover(){
	char * filename = inputRecoverFilename();
	char * currentFileFullname = (char * ) malloc(sizeof(char) * 512);
	int skipBytes = 0, fileBytesSize;
	FILE * chk, * sfs, * currentFile;
	if((chk = fopen("sofilesystem.chk", "r+b")) == NULL) {
		char error[100] = "Erro: Won't be able to read the file";
		strcat(error, "sofilesystem.chk");
		logError(error);
	} else {
		fseek(chk, 0 , SEEK_END);
		if (ftell(chk) != 0){
			fseek(chk, 0, SEEK_SET);
			header headerReader = (header) malloc(sizeof(struct headerInfo));
			fread(&headerReader, sizeof(header), 1, chk);
			file fileRead[headerReader->nFiles];
			fread(&fileRead, sizeof(file), headerReader->nFiles, chk);
			for (int i = 0; i < headerReader->nFiles; i++) {
				if(strcasecmp(filename, fileRead[i]->filename) == 0){
					strcpy(currentFileFullname, fileRead[i]->originPath);
					strcat(currentFileFullname, fileRead[i]->filename);
					fileBytesSize = fileRead[i]->bytesSize;
					file rewriteFile = (file) malloc(sizeof(struct fileInfo));
					rewriteFile = fileRead[i];
					rewriteFile->deleted = false;
					fseek(chk, sizeof(header) + (i * sizeof(file)), SEEK_SET);
					fwrite(&rewriteFile, sizeof(file), 1, chk);
					break;
				} else {
					skipBytes = skipBytes + fileRead[i]->bytesSize;
				}
			}
			if((sfs = fopen("sofilesystem.sfs", "r+b")) == NULL) {
				char error[100] = "Erro: Won't be able to read the file";
				strcat(error, "sofilesystem.sfs");
				logError(error);
			} else {
				fseek(sfs, skipBytes, SEEK_SET);
				if((currentFile = fopen(currentFileFullname, "r+b")) == NULL) {
					char error[100] = "Erro: Won't be able to read the file";
					strcat(error, currentFileFullname);
					logError(error);
				} else {
					char * buffer = (char *)malloc(fileBytesSize * sizeof(char));
					fread(buffer, fileBytesSize, 1 , currentFile);
					fwrite(buffer, sizeof(char), fileBytesSize, sfs);
					free(buffer);
					fprintf(stdout, "+==========================+\n");
					fprintf(stdout, "| Completed recovered file |\n");
					fprintf(stdout, "+==========================+\n");
				}
				fclose(currentFile);
			}
			fclose(sfs);
		} else {
			fprintf(stdout, "+===================================================================+\n");
			fprintf(stdout, "| Your compressed file doesn't have any trace of the requested file |\n");
			fprintf(stdout, "+===================================================================+\n");
		}
		pauseSys();
		clearscr();
	}
	fclose(chk);
}
// endregion [Main Operations]

// region [File Attributes]
char * inputFilename() {
	char * filename = malloc(256);
	printInputFilename();
	fscanf(stdin, " %[^\n]", filename);
	return filename;
}

char * inputRecoverFilename() {
	char * filename = malloc(256);
	printInputRecoverFilename();
	fscanf(stdin, " %[^\n]", filename);
	return filename;
}

char * inputDeleteFilename() {
	char * filename = malloc(256);
	printInputDeleteFilename();
	fscanf(stdin, " %[^\n]", filename);
	return filename;
}

char * inputFilenameReadContent() {
	char * filename = malloc(256);
	printInputFilenameContent();
	fscanf(stdin, " %[^\n]", filename);
	return filename;
}

char * inputOriginPath() {
  	char * filePath = malloc(256);
	printInputOriginPath();
	fscanf(stdin, " %[^\n]", filePath);
	#if defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
		char * currentDir = realpath(filePath, NULL);
		if (currentDir == NULL){
			char error[100] = "Erro: The requested path doesn't exists";
			logError(error);
			exit(1);
		} else {
			free(currentDir);
			return filePath;
		}
	#elif _WIN32
		struct stat buffer;
		int exists = stat(filePath, &buffer);
		if (exists == 0){
			char error[100] = "Erro: The requested path doesn't exists";
			logError(error);
			exit(1);
		} else {
			return filePath;
		}
	#endif
}

char * getExtension(file * newFile) {
	char * filename = (char *) malloc(sizeof(char) * 256);
	strcpy(filename, (*newFile)->filename);
	char * extension;
	extension = strtok(filename, ".");
	extension = strtok(NULL, ".");
	return extension;
}

int getFileSize(file * newFile) {
	struct stat attrib;
	char * filename = (char *) malloc(sizeof(char) * 256);
	strcpy(filename, (*newFile)->originPath);
	strcat(filename, (*newFile)->filename);
	stat(filename, &attrib);
	return (int) attrib.st_size;
}

void getDates(file * newFile, int * modifiedDate, int * creationDate) {
	struct stat attrib;
	char * filename = (char *) malloc(sizeof(char) * 256);
	strcpy(filename, (*newFile)->originPath);
	strcat(filename, (*newFile)->filename);
	stat(filename, &attrib);
	char modifiedDateString[50];
	char creationDateString[50];
	strftime(modifiedDateString, 50, "%Y%m%d", localtime(&attrib.st_mtime));
	strftime(creationDateString, 50, "%Y%m%d", localtime(&attrib.st_ctime));
	*modifiedDate = atoi(modifiedDateString);
	*creationDate = atoi(creationDateString);
}

// endregion [File Attributes]

// region [Check File]
void checkExistingFile(file newFile, bool * exists, bool * recoverable){
	*exists = false;
	*recoverable = false;
	FILE * f;
	if((f = fopen("sofilesystem.chk", "r+b")) == NULL) {
		char error[100] = "Erro: Won't be able to read the file";
		strcat(error, "sofilesystem.chk");
		logError(error);
	} else {
		fseek(f, 0 , SEEK_END);
		if (ftell(f) != 0){
			char * filename = malloc(512);
	  char * fileInside = malloc(512);
			strcpy(filename, newFile->originPath);
			strcat(filename, newFile->filename);
			fseek(f, 0, SEEK_SET);
			header headerReader = (header) malloc(sizeof(struct headerInfo));
			fread(&headerReader, sizeof(header), 1, f);
			file fileRead[headerReader->nFiles];
			fread(&fileRead, sizeof(file), headerReader->nFiles, f);
			for (int i = 0; i < headerReader->nFiles; i++) {
				strcpy(fileInside, fileRead[i]->originPath);
				strcat(fileInside, fileRead[i]->filename);
				if(strcasecmp(filename, fileInside) == 0){
					*exists = true;
					if(fileRead[i]->deleted){
						*recoverable = true;
					}
				}
			}
		}
	}
}
// endregion [Check File]