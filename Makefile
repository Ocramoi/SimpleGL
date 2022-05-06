##
# OpenGL test
#
# @file
# @version 0.1

CC=clang++
SRC_FILES=main.cpp $(wildcard **/*.cpp)
SRC_FILES:=$(filter-out $(wildcard **/flycheck_*.cpp), $(SRC_FILES))
EXEC=build/main
DEXEC=build/debug
COMPILE_FLAGS=-Wextra -Ofast -std=gnu++17
DEBUG_FLAGS=-Wextra -Og -g3 -DDEBUG -std=gnu++17
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes -s
LD_FLAGS=-lGL -lglfw -lGLEW

all: setup
	$(CC) $(COMPILE_FLAGS) $(SRC_FILES) -o $(EXEC) $(LD_FLAGS)

setup:
	rm -rf build
	mkdir build

run:
	./$(EXEC)

debug:
	$(CC) $(DEBUG_FLAGS) $(SRC_FILES) -o $(DEXEC) $(LD_FLAGS)

val:
	valgrind $(VALGRIND_FLAGS) ./$(DEXEC)

# end
