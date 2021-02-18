ALL = tp2virtual
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(wildcard *.c))

CC = gcc

CFLAGS = -g -O0 -Wall 
all: $(ALL)

$(ALL): $(OBJ) 
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) $(OBJ) -o $@ 

clean:
	rm $(ALL) *.o 