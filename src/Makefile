SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
CC = g++ -std=c++17
EXECNAME = tetris
EIGEN_PATH =
SFML_PATH = 



$(EXECNAME) : block.o game.o TileMap.o
	$(CC) block.o TileMap.o game.o -o $(EXECNAME) $(SFML_FLAGS) -O2 $(EIGEN_PATH) $(SFML_PATH)
	rm *.o

game.o :
	$(CC) game.cpp -c $(EIGEN_PATH) $(SFML_FLAGS) $(EIGEN_PATH) $(SFML_PATH)

block.o :
	$(CC) block.cpp -c $(EIGEN_PATH) $(EIGEN_PATH) $(SFML_PATH)

TileMap.o:
	$(CC) TileMap.cpp -c $(EIGEN_PATH) $(SFML_FLAGS) $(EIGEN_PATH) $(SFML_PATH)

clean :
	rm -f *.o
	rm -f $(EXECNAME)



