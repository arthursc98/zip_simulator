main: main.o sysfiles.o helpers.o gui.o
	gcc -o main main.o sysfiles.o helpers.o gui.o

main.o: main.c sysfiles.h gui.h
	gcc -c main.c

sysfiles.o: sysfiles.c sysfiles.h helpers.h
	gcc -c sysfiles.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

gui.o: gui.c gui.h
	gcc -c gui.c