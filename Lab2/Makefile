# Author: Daniel Laritz
# To compile both programs, list all executable names here.
all: server client

# Creates the executable "server" using the flag "-o".
server: server/server.o
	gcc server/server.o -o server/server -g -lm -Wall

# Creates the object file "server.o" using the flag "-c".
server.o: server/server.c
	gcc server/server.c -c -g -lm -Wall

# Creates the executable "client" using the flag "-o".
client: client/client.o
	gcc client/client.o -o client/client -g -lm -Wall

# Creates the object file "client.o" using the flag "-c".
client.o: client/client.c
	gcc client/client.c -c -g -lm -Wall

# Delete object files and executables to start afresh.
# Be sure to delete the "palindrome" executable also.
# Delete these files BEFORE committing your work. 
clean:
	rm -f *.o server/server client/client
