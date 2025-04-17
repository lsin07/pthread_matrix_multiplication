CC=gcc
TARGET=main
# CFLAGS=-I inc -Wall -Wextra -O3 -DDEBUG_LOG
CFLAGS=-I inc -Wall -Wextra -O3
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
