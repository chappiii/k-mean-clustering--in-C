k-mean_cluster: main.o kmeans.c 
	gcc -o k-mean_cluster main.o kmeans.c 

main.o: main.c
	gcc -c main.c
kmeans.o: kmeans.c
	gcc - c kmeans.c