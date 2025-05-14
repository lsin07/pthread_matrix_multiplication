CC=gcc
TARGET=main
CFLAGS=-Iinc -Wall -Wextra
LDFLAGS=-lc -lpthread
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): main.c $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(TARGET) $(OBJ)
