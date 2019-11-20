#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <Eigen/Dense>

#ifndef BLOCK_HPP
#define BLOCK_HPP

using namespace Eigen;

class TetrisBlock
{
public:
  std::string this_name;
  int this_value;
  Vector2f this_pivot;
  Matrix2f this_rotation_matrix;
  Matrix4f this_mask;
  
  TetrisBlock(std::string name, int value, Matrix4f mask, Vector2f pivot);

  Vector2f rotate_point(Vector2f point);
  Matrix4f rotate_mask();
  void rotate();
};

std::ostream & operator<<(std::ostream & flux, TetrisBlock const & block );

bool block_position_possible(int x, int y, const TetrisBlock &block, const MatrixXf &grid);
void add_block_to_grid(int x, int y, const TetrisBlock &block, MatrixXf &grid);
MatrixXf get_visualGrid(int x, int y, int visualNrows, int visualNcols, const TetrisBlock &block, MatrixXf grid, int top_padding, int left_padding);

int next_one_index(int index, VectorXf col);
bool is_line(VectorXf row);
std::vector<int> find_lines(const MatrixXf &grid, int padding);
VectorXf refactor_col(int line_index, VectorXf col);
void delete_line(int line_index, MatrixXf &grid, int padding);


std::unordered_map<std::string, TetrisBlock> get_block_map();

#endif


