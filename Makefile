CC = gcc
CFLAGS = -g -pthread
OBJ = matrix.o main.o subtotal.o
EXEC = pmms

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

main.o : main.c macros.h
	$(CC) -c main.c

matrix.o : matrix.c macros.h
	$(CC) -c matrix.c

subtotal.o : subtotal.c macros.h
	$(CC) -c subtotal.c

clean:
	rm -f $(EXEC) $(OBJ)
