################################################
# Makefile Variables                           #
################################################
CC = cc
COMPILER_FLAGS = -Wall -g -Wfatal-errors
SRC_FILES = src/*.c
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -llua5.4
OBJ_NAME = gameengine


################################################
# Makefile Rules                               #
################################################
build:
	$(CC) $(COMPILER_FLAGS) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME) 