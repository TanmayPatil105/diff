diff: main.o diff.o
	gcc -o diff main.o diff.o

main.o: main.c diff.h
	gcc -c main.c diff.h

diff.o: diff.c
	gcc -c diff.c