kmeans: main.o kmeans.o
	gcc -g -o kmeans main.o kmeans.o -lm

main.o: main.c
	gcc -g -c main.c

kmeans.o: kmeans.c
	gcc -g -c kmeans.c

clean:
	rm -f *.o kmeans
