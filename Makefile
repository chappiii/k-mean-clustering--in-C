# Variables for flags
CFLAGS = -g -O0 -Wall -Wextra -pedantic
LDFLAGS = -lm

# Targets
kmeans: main.o kmeans.o
	gcc $(CFLAGS) -o kmeans main.o kmeans.o $(LDFLAGS)
	rm -f *.o
	valgrind --leak-check=full ./kmeans  # Automatically runs Valgrind after building

main.o: main.c
	gcc $(CFLAGS) -c main.c

kmeans.o: kmeans.c
	gcc $(CFLAGS) -c kmeans.c

clean:
	rm -f *.o kmeans
