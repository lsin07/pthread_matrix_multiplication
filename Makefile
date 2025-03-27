CC=gcc
TARGET=main
# CFLAGS=-I inc -lpthread -Wall -DDEBUG_LOG
CFLAGS=-I inc -lpthread -Wall
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): main.c $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(TARGET) $(OBJ)
