##
# OpenGL test
#
# @file
# @version 0.1

CC=clang++
SRC_FILES=main.cpp $(wildcard **/*.cpp)
SRC_FILES:=$(filter-out $(wildcard **/flycheck_*.cpp), $(SRC_FILES))
EXEC= ./build/main
DEXEC=../build/debug
OBJ_FOLDER=objs
COMPILE_FLAGS=-Wextra -Ofast -std=gnu++17
DEBUG_FLAGS=-Wextra -DDEBUG -std=gnu++17
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes -s
LD_FLAGS:=-lGL -lglfw -lGLEW -lSOIL

all: classes.o utils.o main.o
	$(CC) $(COMPILE_FLAGS) *.o -o $(EXEC) $(LD_FLAGS)

main.o: main.cpp
	$(CC) $(COMPILE_FLAGS) main.cpp -c

classes.o: $(filter-out $(wildcard **/flycheck_*.cpp), $(wildcard classes/*.cpp))
	$(CC) $(COMPILE_FLAGS) $(filter-out $(wildcard **/flycheck_*.cpp), $(wildcard classes/*.cpp)) -c

utils.o: $(filter-out $(wildcard **/flycheck_*.cpp), $(wildcard utils/*.cpp))
	$(CC) $(COMPILE_FLAGS) $(filter-out $(wildcard **/flycheck_*.cpp), $(wildcard utils/*.cpp)) -c

setup:
	rm -rf build
	mkdir build

run:
	./$(EXEC)

debug:
	$(CC) $(DEBUG_FLAGS) main.cpp $(filter-out $(wildcard **/flycheck_*.cpp), $(wildcard **/*.cpp)) -o $(DEXEC) $(LD_FLAGS)

val:
	valgrind $(VALGRIND_FLAGS) ./$(DEXEC)

clear:
	rm *.o

.PHONY: all clear val run setup

# end
