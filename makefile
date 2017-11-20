CC=gcc
CFLAGS=-Wall
OBJ=cipher.o main.o

util:	$(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o Projet_S3/xorcipher

Projet_S3/cipher.o: cipher.c
Projet_S3/main.o: main.c

clean:
	rm -rf *.o

maxclean: clean
	  rm -rf $(EXEC)
