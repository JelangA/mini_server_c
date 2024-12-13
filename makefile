CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/main.c src/child.c src/client.c src/log.c src/signal_handler.c src/url_decoder.c
OBJ = $(SRC:.c=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

