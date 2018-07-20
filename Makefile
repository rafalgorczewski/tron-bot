CC=g++
BASE_FLAGS=-std=c++17 -Wall -Wextra
INCLUDE_PATH=include/
SRC_PATH=src/

ifeq ($(MODE), release)
	FLAGS=$(BASE_FLAGS) -Ofast
else
	ifeq ($(MODE), debug)
		FLAGS=$(BASE_FLAGS) -O0 -g
	endif
endif

rafal.out : MapParser.o MovementSolver.o Bfs.o WallRunner.o rafal.o
	$(CC) $^ -o $@

rafal.o : rafal.cpp \
$(SRC_PATH)MapParser.cpp \
$(INCLUDE_PATH)MapParser.hpp \
$(INCLUDE_PATH)MapData.hpp \
$(INCLUDE_PATH)Vector2i.hpp \
$(SRC_PATH)MovementSolver.cpp \
$(INCLUDE_PATH)MovementSolver.hpp \
$(INCLUDE_PATH)Bfs.hpp \
$(SRC_PATH)Bfs.cpp \
$(INCLUDE_PATH)Direction.hpp
	$(CC) $(FLAGS) -c rafal.cpp -I $(INCLUDE_PATH)

MapParser.o : $(SRC_PATH)MapParser.cpp \
$(INCLUDE_PATH)MapParser.hpp \
$(INCLUDE_PATH)MapData.hpp \
$(INCLUDE_PATH)Vector2i.hpp
	$(CC) $(FLAGS) -c $(SRC_PATH)MapParser.cpp -I $(INCLUDE_PATH)

MovementSolver.o : $(SRC_PATH)MovementSolver.cpp \
$(INCLUDE_PATH)MovementSolver.hpp \
$(INCLUDE_PATH)MapData.hpp \
$(INCLUDE_PATH)Vector2i.hpp \
$(INCLUDE_PATH)Bfs.hpp \
$(SRC_PATH)Bfs.cpp \
$(INCLUDE_PATH)Direction.hpp
	$(CC) $(FLAGS) -c $(SRC_PATH)MovementSolver.cpp -I $(INCLUDE_PATH)

Bfs.o : $(INCLUDE_PATH)MapData.hpp \
$(INCLUDE_PATH)Vector2i.hpp \
$(SRC_PATH)Bfs.cpp \
$(INCLUDE_PATH)Direction.hpp
	$(CC) $(FLAGS) -c $(SRC_PATH)Bfs.cpp -I $(INCLUDE_PATH)

WallRunner.o : $(INCLUDE_PATH)MapData.hpp \
$(INCLUDE_PATH)Vector2i.hpp \
$(SRC_PATH)Bfs.cpp \
$(INCLUDE_PATH)Direction.hpp \
$(SRC_PATH)WallRunner.cpp \
$(INCLUDE_PATH)WallRunner.hpp
	$(CC) $(FLAGS) -c $(SRC_PATH)WallRunner.cpp -I $(INCLUDE_PATH)

clean :
	rm -r MapParser.o MovementSolver.o Bfs.o WallRunner.o rafal.o rafal.out
