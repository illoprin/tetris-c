# SimpleTetris Makefile
# Собрать все исходники программы *.c в отдельные tmp/*.obj файлы, а далее объединить tmp/*.obj файлы в полноценный бинарник и запустить его

HI_MSG=echo "Powered by Loprin Branding Design\nCopyright (c) 2024 Ilya Kuntsevich (illoprin)"

CC=gcc # using gcc compiler

CFLAGS=-Wall # flags: All warnings on, ANSI C standart - C99 - bullshit (no unix sleep function)
LDFLAGS= # linker flags

SRC_DIR=src # src directory

# compiled sources (objects) directory
OBJ_DIR=tmp

TARGET=bin/app # executable bin

BIN_DIR:=$(dir $(TARGET))

# SRCS=src/main.c # compile main.c only

SRCS=$(shell find $(SRC_DIR) -name '*.c') # find all *.c files by name

OBJS=$(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:%.c=%.o))) # get all obj file links based on name of src file eg: main.c -> main.o

LIBS=-lncurses -lm

$(shell mkdir -p $(OBJ_DIR)) # create tmp dir if not exists
$(shell mkdir -p $(BIN_DIR)) # create bin dir if not exists

run:
	$(HI_MSG)
	$(CC) $(CFLAGS) -g $(SRCS) -o $(TARGET) $(LIBS)
	./$(TARGET)

clear:
	$(HI_MSG)
	rm -drf $(OBJ_DIR)
	rm -drf $(BIN_DIR)
	echo "Cleared!"
