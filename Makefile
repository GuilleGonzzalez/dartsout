PROJECT_NAME := dartsout

SRC_DIR := src
INC_DIR := include
OUT_DIR := build

CC=gcc
MK=mkdir
RM=rm -rf

CFLAGS=-I$(INC_DIR)
CFLAGS+=-Ilib/mongoose
CFLAGS+=-Os
CFLAGS+=-Wall
CFLAGS+=-Werror
CFLAGS+=-Wextra
CFLAGS+=-pedantic
CFLAGS+=-DMG_ENABLE_ASSERT=1


CFLAGS+=-DLOG_USE_COLOR

LIBS=-lsqlite3
LIBS+=-lcjson

OUTPUT_FILE=$(PROJECT_NAME)

.PHONY: clean all

all: $(OUTPUT_FILE).out

# wildcard proporciona un string con los *.c separados por un espacio
# patsubst reemplaza todos los *.c por *.o
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SRC_FILES += lib/mongoose/mongoose.c
OBJ_FILES := $(foreach sf, $(SRC_FILES), \
	$(eval of := $(OUT_DIR)/$(notdir $(sf).o)) \
	$(eval $(of)_SRC := $(sf)) \
	$(eval $(of): $($(of)_SRC)) \
	$(of))

%.c.o: $(OUT_DIR)
	$(info Generating: $@)
	$(CC) $(CFLAGS) -c $($@_SRC) -o $@

$(OUTPUT_FILE).out: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBS) -o $@

$(OUT_DIR):
	$(MK) $(OUT_DIR)

clean:
	$(RM) $(PROJECT_NAME).out
	$(RM) $(OUTPUT_FILE)
	$(RM) $(OUT_DIR)
	$(RM) compile_commands.json

CLANG_ROOT := $(shell pwd)

.PHONY: compile_commands.json

compile_commands.json: $(SRC_FILES) Makefile
	$(info Generating: $@)
	$(file >$@,[)
	$(foreach F,$(SRC_FILES), \
	$(file >>$@,    {) \
	$(file >>$@,            "command": "$(CC) -c $(CFLAGS) $(F)",) \
	$(file >>$@,            "directory": "$(CLANG_ROOT)",) \
	$(file >>$@,            "file": "$(F)") \
	$(file >>$@,    }$(if $(filter $(word $(words $(SRC_FILES)),$(SRC_FILES)),$(F)),,,)) \
	)
	$(file >>$@,])
	@:

