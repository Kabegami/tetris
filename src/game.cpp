#include "block.hpp"
#include "TileMap.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <ostream>

using namespace Eigen;

bool updateMove(int move, int &x, const int &y, const TetrisBlock &block, const MatrixXf &grid){
  /* Try to move the block horizontaly and return a boolean indicating of the operation was a succes or not*/
  
  if (block_position_possible(x + move, y, block, grid)){
    x += move;
    return true;
  }
  return false;
}

bool checkRotation(const int &x, const int &y, TetrisBlock block, const MatrixXf &grid){
  /* Check if a rotation is possible, it doesn't do any side effect */
  
  block.rotate();
  return block_position_possible(x, y, block, grid);
}

bool updateFalling(sf::Clock &clock, int &x, int &y, const TetrisBlock &block, MatrixXf &grid, float FALLING_TIME, float falling_multiplicator){
  /* This fonction handle the folling of a piece. It return false if the block can move or not enought time is elapsed and true if the block can't be placed. It change the position of y.
   */
  
  sf::Time elapsed = clock.getElapsedTime();
  if (elapsed.asSeconds() < FALLING_TIME * falling_multiplicator){
    return false;
  }
  clock.restart();
  // std::cout << x << "," << y << std::endl;
  if (block_position_possible(x, y+1, block, grid)){
    y += 1;
    return false;
  }
 
  add_block_to_grid(x, y, block, grid);

  return true;
  
}

int main() {
  srand (time(NULL));

  // simple types init
  const float FALLING_TIME = 0.5;
  float falling_multiplicator = 1;
  //2 top padding, 3 bottom padding
  const int nrows = 25;
  const int ncols = 16;
  const int visualNrows = 20;
  const int visualNcols = 10;
  const int xspawn = (ncols / 2) - 1;
  const int yspawn = 0;
  const int visualSize = 25;
  const int tileSize = 100;
  int x = xspawn;
  int y = yspawn;
  int bottom_padding = 2;
  int col_padding = 3;
  bool block_down = false;
  int move = 0;
  int line_index;
  bool gameOver = false;
  std::string fname {"../img/spritesheet.png"};

  // complexe type init
  MatrixXf grid(nrows, ncols);
  MatrixXf visualGrid(visualNrows, visualNcols);
  sf::RenderWindow window(sf::VideoMode(visualSize * visualNcols, visualSize * visualNrows), "Tetris");
  TileMap map;
  std::vector<int> line_indexes;
  sf::Clock clock;
  //get_block_map if defined in block.cpp
  auto block_map = get_block_map();

  // fonctions
  
  auto randint = [](int maxValue) -> int {return rand() % (maxValue); };

  auto get_random_block = [block_map, randint]() -> TetrisBlock {
			    auto random_it = std::next(std::begin(block_map), randint(block_map.size()));
			    return (*random_it).second;};


  //We use padding to handle contact with the sides of the grid
  auto init_grid = [](MatrixXf &grid, int nrows, int ncols) -> void {
    grid = MatrixXf::Zero(nrows, ncols);
    // bottom padding
    grid.row(nrows-1) = VectorXf::Ones(ncols);
    grid.row(nrows-2) = VectorXf::Ones(ncols);
    
    // left padding
    grid.col(0) = VectorXf::Ones(nrows);
    grid.col(1) = VectorXf::Ones(nrows);
    grid.col(2) = VectorXf::Ones(nrows);

    // right padding
    grid.col(ncols-1) = VectorXf::Ones(nrows);
    grid.col(ncols-2) = VectorXf::Ones(nrows);
    grid.col(ncols-3) = VectorXf::Ones(nrows);
      };

  auto delete_all_lines = [bottom_padding, col_padding](MatrixXf &grid, std::vector<int> &line_indexes) -> void {
				  int index;
			     	  line_indexes = find_lines(grid, bottom_padding);
				  while (!line_indexes.empty()){
				    index = line_indexes.back();
				    delete_line(index, grid, col_padding);
				    line_indexes.pop_back();    
				  }
				  line_indexes = find_lines(grid, bottom_padding);
				};
  

  init_grid(grid, nrows, ncols);
  TetrisBlock block = get_random_block();
  
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
	  if (event.type == sf::Event::Closed)
                window.close();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
	  move = -1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
	  move = 1;
	}

	if (sf::Keyboard::isKeyPressed((sf::Keyboard::Up))){
	  move = 2;
	}

	if (sf::Keyboard::isKeyPressed((sf::Keyboard::Down))){
	  move = 3;
	}
	

      }

	falling_multiplicator = 1;



	if ((move == -1) || (move == 1)){
	  updateMove(move, x, y, block, grid);
	}

	if (move == 2){
	  if (checkRotation(x, y, block, grid)){
	    block.rotate();
	  }
	}

	if (move == 3){
	  falling_multiplicator = 0.15;
	}
	
	move = 0;

	block_down = updateFalling(clock, x, y, block, grid, FALLING_TIME, falling_multiplicator);

	if (!line_indexes.empty()){
	  delete_all_lines(grid, line_indexes);
	}
	
	if (block_down){
	  //create a new block
	  x = xspawn;
	  y = yspawn;
	  block = get_random_block();
	  
	  if (!block_position_possible(x, y, block, grid)){
	    gameOver = true;
	  }
	  delete_all_lines(grid, line_indexes);
	}

	if (gameOver){
	    std::cout << "Game over !" << std::endl;
	    window.close();
	  }
	 
	visualGrid = get_visualGrid(x, y, visualNrows, visualNcols,  block, grid, 3, 3);

	map.load(fname, sf::Vector2u(tileSize, tileSize), sf::Vector2u(visualSize, visualSize), visualGrid);
	
        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
    	window.draw(map);
        // end the current frame
        window.display();
    }

      return 0;
 
}
