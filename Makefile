# CSE-232 TERM PROJECT

# === Project Settings ===
TARGET_NAME := cse232_editor
TARGET_EXT  := 
TARGET      := bin/$(TARGET_NAME)$(TARGET_EXT)

LIBRARY_NAME := libed2.a
LIBRARY      := obj/$(LIBRARY_NAME)

MAINFILE	:= main

TESTFILE 	:= TEST.txt

INC_DIR     := inc
SRC_DIR     := src
OBJ_DIR     := obj
BIN_DIR     := bin

AR          := ar
CC          := gcc
CFLAGS      := -O2 -I$(INC_DIR)

ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG
endif

LDFLAGS      := -lncurses

FILES   	:= AlpHasanSimsek PoyrazErdemKoyuncu FurkanAhmetYagci ErtugrulCevik MuhammetFurkanUzun EmreAyik

# === Source and Object Files ===
SRC_FILES   := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES   := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# === Build Rules ===
.PHONY: all run clean directories

all: directories $(TARGET)

$(TARGET): $(LIBRARY)
	$(CC) $(CFLAGS) -c $(MAINFILE).c -o $(MAINFILE).o
	$(CC) $(MAINFILE).o $^ -o $@ $(LDFLAGS)

# Rule to create the static library
$(LIBRARY): $(OBJ_FILES)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TESTFILE):
	touch  $(TESTFILE)
	echo "!Test File" >> $(TESTFILE)
	echo "deaddead" >> $(TESTFILE)
	echo "beafbeaf" >> $(TESTFILE)

run: $(TARGET) $(TESTFILE)
	./$(TARGET) $(TESTFILE)

init:
	@for file in $(FILES); do \
		touch $(SRC_DIR)/$$file.c; \
	done
	@echo "Initialized: $(FILES)"

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(MAINFILE).o
