all: client.o server.o
	gcc -o client client.c
	gcc -o server server.c

client.o: client.c
	gcc -c client.c

server.o: server.c
	gcc -c server.c

run: 
	./client & ./server
	
clean:
	rm *.o
	rm server
	rm client