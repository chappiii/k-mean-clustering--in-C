# Variables for flags
CFLAGS = -g -O0 -Wall -Wextra -pedantic  # -g flag for debugging with gdb, others for warnings and no optimization
LDFLAGS = -lm  # Links the math library

# Main target: Builds the kmeans program by linking object files
# Usage: Run `make` to compile and link the program
kmeans: main.o kmeans.o
	gcc $(CFLAGS) -o kmeans main.o kmeans.o $(LDFLAGS)

# Rule to compile main.c into main.o
# Usage: Automatically invoked when main.o is required
main.o: main.c
	gcc $(CFLAGS) -c main.c

# Rule to compile kmeans.c into kmeans.o
# Usage: Automatically invoked when kmeans.o is required
kmeans.o: kmeans.c
	gcc $(CFLAGS) -c kmeans.c

# Clean up target: Removes object files and the executable
# Usage: Run `make clean` to remove generated files
clean:
	rm -f *.o kmeans

# Target to run the program with Valgrind to check for memory leaks
# Usage: Run `make valgrind` to execute the program with Valgrind
valgrind: kmeans
	valgrind --leak-check=full ./kmeans

# Target to run the program with gdb for debugging
# Usage: Run `make debug` to execute the program in gdb
debug: kmeans
	gdb ./kmeans
