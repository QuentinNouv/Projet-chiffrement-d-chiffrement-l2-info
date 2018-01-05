CC=gcc
CFLAGS=-Wall
OBJ=dico.o freq.o copy.o cipher.o valid.o main.o

util:	$(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o xorcipher -Ofast -Wextra -g -lm

dico.o: dico.c
freq.o: freq.c
copy.o: copy.c
cipher.o: cipher.c
valid.o : valid.c
main.o: main.c

clean:
	rm -rf *.o

maxclean: clean
	  rm -rf $(EXEC)
