CC=gcc
CFLAGS=-Wall
OBJ=cipher.o valid.o main.o

util:	$(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o Projet_S3/xorcipher

cipher.o: cipher.c
valid.o : valid.c
main.o: main.c

clean:
	rm -rf *.o

maxclean: clean
	  rm -rf $(EXEC)
