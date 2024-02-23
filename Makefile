PROJECT_NAME := darts

SRC_DIR := src
INC_DIR := include
OUT_DIR := build

CC=gcc
MK=mkdir
RM=rm -rf

CFLAGS=-I$(INC_DIR)
CFLAGS+=-Os
CFLAGS+=-Wall
CFLAGS+=-Werror
CFLAGS+=-Wextra
CFLAGS+=-pedantic

LIBS=

OUTPUT_FILE=$(PROJECT_NAME)

.PHONY: clean

all: $(OUTPUT_FILE).out

# wildcard proporciona un string con los *.c separados por un espacio
# patsubst reemplaza todos los *.c por *.o
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_FILE).out: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBS) -o $@

clean:
	$(RM) $(OBJ_FILES)
	$(RM) $(OUTPUT_FILE)