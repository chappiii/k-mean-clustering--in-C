kmeans: main.o kmeans.o
	gcc -o kmeans main.o kmeans.o -lm

main.o: main.c
	gcc -c main.c

kmeans.o: kmeans.c
	gcc -c kmeans.c

clean:
	rm -f *.o kmeans
