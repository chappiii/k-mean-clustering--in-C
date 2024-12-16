# Variables for flags
CFLAGS = -g -O0 -Wall -Wextra -pedantic  # -g flag for debugging with gdb, others for warnings and no optimization
LDFLAGS = -lm  # Links the math library

# Target: Builds the kmeans program by linking object files
# Usage: Run `make` to compile and link the program and run `./kmeans` to execute the code
kmeans: main.o kmeans.o
	gcc $(CFLAGS) -o kmeans main.o kmeans.o $(LDFLAGS)

# Target: Rule to compile main.c into main.o
# Usage: Automatically invoked when main.o is required
main.o: main.c
	gcc $(CFLAGS) -c main.c

# Target: Rule to compile kmeans.c into kmeans.o
# Usage: Automatically invoked when kmeans.o is required
kmeans.o: kmeans.c
	gcc $(CFLAGS) -c kmeans.c

# Target: Removes object files and the executable
# Usage: Run `make clean` to remove generated files
clean:
	rm -f *.o kmeans

# Target: to run the program with Valgrind to check for memory leaks and time to check memory usage and execution time
# Usage: First run `sudo dnf install valgrind` to install valgrind and Run `make valgrind` to execute the program with Valgrind and check execution time and memory usage
valgrind: kmeans
	time valgrind --leak-check=full ./kmeans

# Target: to run the program with gdb for debugging
# Usage: First run `sudo dnf install gdb` to install gdb and Run `make debug` to execute the program in gdb for debugging
debug: kmeans
	gdb ./kmeans
