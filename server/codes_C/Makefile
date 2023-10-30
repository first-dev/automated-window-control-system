CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g
LDFLAGS = -lm
EXEC = soft_embarque
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: keep_as_cool_as_possible control_window record_temp

keep_as_cool_as_possible:
	$(CC) -o keep_as_cool_as_possible.o keep_as_cool_as_possible.c timing.h pi_mmio.h pi_mmio.c dht11.h dht11.c 28byj-48.h 28byj-48.c -lsqlite3
	
control_window:
	$(CC) -o control_window.o control_window.c timing.h pi_mmio.h pi_mmio.c dht11.h dht11.c 28byj-48.h 28byj-48.c -lsqlite3

record_temp:
	$(CC) -o record_temp.o record_temp.c timing.h pi_mmio.h pi_mmio.c dht11.h dht11.c 28byj-48.h 28byj-48.c -lsqlite3

