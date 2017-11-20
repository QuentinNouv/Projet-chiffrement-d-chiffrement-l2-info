CC=gcc
CFLAGS=-Wall
OBJ=cipher.o main.o

util:	$(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o xorcipher

cipher.o: cipher.c
main.o: main.c

clean:
	rm -rf *.o

maxclean: clean
	  rm -rf $(EXEC)
