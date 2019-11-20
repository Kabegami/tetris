#include "block.hpp"
#include <iostream>
#include <Eigen/Dense>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace Eigen;


TetrisBlock::TetrisBlock(std::string name, int value, Matrix4f mask, Vector2f pivot){
    this_name = name;
    this_pivot = pivot;
    this_mask = mask;
    this_rotation_matrix << 0, -1, 1, 0;
    this_value = value;
};

Vector2f TetrisBlock::rotate_point(Vector2f point){
    return (this_rotation_matrix * (point - this_pivot)) + this_pivot;
}

Matrix4f TetrisBlock::rotate_mask(){
    Matrix4f new_mask = Matrix4f::Zero();
    int newx;
    int newy;
    auto handle_negative_index = [](int index){
			    if (index < 0){
			      index = 4 + index;
			    }
			    return index;
			  };

    
    for (int i=0; i < 4; i++){	
      for (int j=0; j < 4; j++){
	if (this_mask(i, j) != 0){
	    Vector2f point;
	    point << i, j;
	    auto position = rotate_point(point);
	    newx = handle_negative_index(position(0));
	    newy = handle_negative_index(position(1));
	    new_mask(newx, newy) = this_value;
	  }
      }
    }
    return new_mask;
  }

void TetrisBlock::rotate(){
  this_mask = rotate_mask();
}  


std::ostream & operator<<(std::ostream & flux, TetrisBlock const & block ){
  return flux << "\n" <<  "block : " << block.this_name << "\n" << "----------\n" << block.this_mask << "\n";
}


bool block_position_possible(int x, int y, const TetrisBlock &block, const MatrixXf &grid){
  /* Check if the block can be placed in the position (x,y), with (x,y) reprenting the left corner of the mask*/
  
  auto mask_block = block.this_mask.block(0, 0, 4, 4);
  auto grid_block = grid.block(y, x,  4, 4);
  auto bool_block = (mask_block.array() * grid_block.array()) != 0.0;
  return !(bool_block.any());
}


void add_block_to_grid(int x, int y, const TetrisBlock &block, MatrixXf &grid){
  /* Add the block to the grid. This function do some side effects on grid
   */
  auto mask_block = block.this_mask.block(0, 0, 4, 4);
  auto grid_block = grid.block(y, x,  4, 4);
  grid_block = mask_block + grid_block;
}




MatrixXf get_visualGrid(int x, int y, int visualNrows, int visualNcols, const TetrisBlock &block, MatrixXf grid, int top_padding, int left_padding){
  /* Return the visual grid wich is used to display the game to the player without showing the paddings blocks, it doesn't do any side effects*/
  auto mask_block = block.this_mask.block(0, 0, 4, 4);
  auto grid_block = grid.block(y, x,  4, 4);
  grid_block = mask_block + grid_block;
  
  auto visualGridBlock = grid.block(3, 3, visualNrows, visualNcols);
  MatrixXf newGrid (visualNrows, visualNcols);
  newGrid = visualGridBlock;
  return newGrid;
}

int next_one_index(int index, VectorXf col){
  for (int i =0; i < col.size(); i++){
    if (i > index){
      if (col(i) != 0){
	return i;
      }
    }
  }
  // if no values are found return a negative value;
  return -1;
}

bool is_line(VectorXf row) { return row.array().all(); }

std::vector<int> find_lines(const MatrixXf & grid, int padding){
  /* Return a vector of row indexes of completed lines*/
  std::vector<int> indexes;
  for (int i = 0; i < grid.rows() - padding; i++){

    if (is_line(grid.row(i))){
      indexes.push_back(i);
    }
  }
  return indexes;
}


VectorXf refactor_col(int line_index, VectorXf col){
  /* Construct a new column by destroying the line value and shifting the values to the next non-zero value (make the block "fall").*/
  
  int inext = next_one_index(line_index, col);
  VectorXf new_col(col.size());
  new_col << VectorXf::Zero(inext - line_index), col.head(line_index), col.tail(col.size() - inext);
  return new_col;
}

void delete_line(int line_index, MatrixXf &grid, int padding){
  /* Delete a line by refactoring all the columns of the grid who are not padding columns*/
  for (int j = padding; j < grid.cols() - padding; j++){
    auto col = grid.col(j);
    col = refactor_col(line_index, col);
  }
}



std::unordered_map<std::string, TetrisBlock> get_block_map(){
  std::unordered_map<std::string, TetrisBlock> map;
  
  // I, cyan
  Matrix4f Imask;
  Vector2f Ipivot;
  Imask << 0, 0, 0, 0,
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0;

  Ipivot << 2, 1;
  map.insert({"I", TetrisBlock("I", 1, Imask, Ipivot)});

  // 
  Matrix4f Omask;
  Vector2f Opivot;
  Omask << 0, 0, 0, 0,
    0, 0, 0, 0,
    0, 2, 2, 0,
    0, 2, 2, 0;

  Opivot << 2.5, 1.5;
  map.insert({"O", TetrisBlock("O", 2, Omask, Opivot)});

  Matrix4f Tmask;
  Vector2f Tpivot;
  Tmask << 0, 0, 0, 0,
    0, 0, 0, 0,
    3, 3, 3, 0,
    0, 3, 0, 0;

  Tpivot << 2, 1;
  map.insert({"T", TetrisBlock("T", 3, Tmask, Tpivot)});

  Matrix4f Lmask;
  Vector2f Lpivot;
  Lmask << 0, 0, 0, 0,
    0, 0, 0, 0,
    4, 4, 4, 0,
    4, 0, 0, 0;

  Lpivot << 2, 1;
  map.insert({"L", TetrisBlock("L", 4, Lmask, Lpivot)});

  Matrix4f Jmask;
  Vector2f Jpivot;
  Jmask << 0, 0, 0, 0,
    0, 0, 0, 0,
    5, 5, 5, 0,
    0, 0, 5, 0;

  Jpivot << 2, 1;
  map.insert({"J", TetrisBlock("J", 5, Jmask, Jpivot)});

  Matrix4f Zmask;
  Vector2f Zpivot;
  Zmask << 0, 0, 0, 0,
    6, 6, 0, 0,
    0, 6, 6, 0,
    0, 0, 0, 0;

  Zpivot << 2, 1;
  map.insert({"Z", TetrisBlock("Z", 6, Zmask, Zpivot)});

  Matrix4f Smask;
  Vector2f Spivot;
  Smask << 0, 0, 0, 0,
    0, 7, 7, 0,
    7, 7, 0, 0,
    0, 0, 0, 0;

  Spivot << 2, 1;
  map.insert({"S", TetrisBlock("S", 7, Smask, Spivot)});
  
  return map;
  
}


 
