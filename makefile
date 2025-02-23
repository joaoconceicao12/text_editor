CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS =

SRC = editor.c
OBJ = $(SRC:.c=.o)
EXEC = editor

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)