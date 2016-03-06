CC=gcc -std=c99
EXEC=main

# Variables needed by default.mk

ALL = $(EXEC)

# targets

./bin/main : ./bin/main.o ./bin/Affichage.o ./bin/Operation.o
	$(CC) -o ./bin/main ./bin/main.o ./bin/Affichage.o ./bin/Operation.o

./bin/main.o: ./src/Main.c
	$(CC) -o ./bin/main.o -c ./src/Main.c

./bin/Affichage.o: ./src/Affichage.c
	$(CC) -o ./bin/Affichage.o -c ./src/Affichage.c

./bin/Operation.o: ./src/Operation.c
	$(CC) -o ./bin/Operation.o -c ./src/Operation.c

clean:
	rm -rf ./bin/*.o




