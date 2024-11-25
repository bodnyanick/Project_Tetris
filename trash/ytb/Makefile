TARGET = SimpleTetris
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c11 -D_POSIX_C_SOURCE=199309L
LDFLAGS = -lncurses -lrt

PREF_SRC = ./src2/
PREF_OBJ = ./obj/

SRC = $(shell find . -name "*.c")
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(PREF_OBJ)%.o: $(PREF_SRC)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)