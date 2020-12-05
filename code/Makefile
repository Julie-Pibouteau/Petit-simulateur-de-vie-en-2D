
CC=gcc
CFLAGS=-Wall -O3
#LDFLAGS=-lm
LDFLAGS=
EXEC=animauphine


all: $(EXEC)

animauphine: main.o animal.o list.o world.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

main.o: main.c animal.h list.h world.h
	$(CC) -o $@ -c $< $(CFLAGS)

animal.o: animal.c animal.h
	$(CC) -o $@ -c $< $(CFLAGS)

list.o: list.c list.h
	$(CC) -o $@ -c $< $(CFLAGS)

world.o: world.c world.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o core sortie.phine

mrproper: clean
	rm -f $(EXEC)
