# Variables for flags
CFLAGS = -g -O0 -Wall -Wextra -pedantic  # -g flag for debugging with gdb
LDFLAGS = -lm

# Targets for building the kmeans program
kmeans: main.o kmeans.o
	gcc $(CFLAGS) -o kmeans main.o kmeans.o $(LDFLAGS)

main.o: main.c
	gcc $(CFLAGS) -c main.c

kmeans.o: kmeans.c
	gcc $(CFLAGS) -c kmeans.c

# Clean up object files and the executable
clean:
	rm -f *.o kmeans

# Dedicated target for running Valgrind
valgrind: kmeans
	valgrind --leak-check=full ./kmeans

# Dedicated target for running the program with gdb
debug: kmeans
	gdb ./kmeans
