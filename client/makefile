CC=g++ -ggdb
CFLAGS=-std=c++11
LIBS=-lpthread
DEPS=client.c getArgs.c
OBJECTS=client.x getArgs.x

c.x: $(OBJECTS)
	$(CC) -o c.x $(OBJECTS) $(CFLAGS) $(LIBS)

client.x: client.c
	$(CC) -c client.c -o client.x $(CFLAGS) $(LIBS)

getArgs.x: getArgs.c
	$(CC) -c getArgs.c -o getArgs.x $(CFLAGS) $(LIBS)

clean:
	rm *.o *.x *~ *#


