#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

int main(){
  const int nrows = 20;
  const int ncols = 10;

  MatrixXd grid(nrows, ncols);
  grid = MatrixXd::Zero(nrows, ncols);

  std::cout << grid << std::endl;
  
  return 0;
}
