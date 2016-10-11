CC=g++ -ggdb
CFLAGS=-std=c++11
LIBS=-lpthread
DEPS=server.c getArgs.c client.c file.c
OBJECTS=server.x getArgs.x client.x file.x

s.x: $(OBJECTS)
	$(CC) -o s.x $(OBJECTS) $(CFLAGS) $(LIBS)

server.x: server.c
	$(CC) -c server.c -o server.x $(CFLAGS) $(LIBS) 

getArgs.x: getArgs.c
	$(CC) -c getArgs.c -o getArgs.x $(CFLAGS) $(LIBS) 

client.x: client.c
	$(CC) -c client.c -o client.x $(CFLAGS) $(LIBS) 

file.x: file.c
	$(CC) -c file.c -o file.x $(CFLAGS) $(LIBS) 


# don't remove .o files created by the implicit rule above                      
# .PRECIOUS: %.o

clean:
	rm *.o *.x *~ *#

