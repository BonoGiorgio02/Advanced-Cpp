#include <iostream>
#include <complex>
#include "Matrix.hpp"

using namespace matrix;

// Set the next macro to the question number you want to test

#define QUESTION 4

int main(int argc, char** argv) {
  // Test matrix construction
  Matrix<4,3> M1 = 1.;
  Matrix<4,3> M2 = M1;
  M2[{2,1}] = 0.;

#if QUESTION == 1
  std::cout << "\nMatrix construction and operator[] compile but we can't print anything at this stage.\n" << std::endl;
#endif
    
#if QUESTION >= 2
  // Test matrix serialization
  std::cout << "M1 = " << std::endl << M1 << std::endl;
  std::cout << "M2 = " << std::endl << M2 << std::endl;
#endif
  
#if QUESTION >= 3
  // Test matrix addition
  std::cout << "M1 + M2 = " << std::endl << M1 + M2 << std::endl;
#endif
  
#if QUESTION >= 4
  // Test matrix multiplication
  Matrix<3,2> M3 = 1.;
  M3[{0,0}] = 0.;
  std::cout << "M3 = " << std::endl << M3 << std::endl;
  std::cout << "M2 * M3 = " << std::endl << M2 * M3 << std::endl;
#endif

#if QUESTION >= 5  
  // Test multiplication code branching
  {
    Matrix<3,1> V = 1.;
    Matrix<1,3> L = 2.;
    std::cout << "V * L = " << std::endl << V * L << std::endl;
  }
#endif

#if QUESTION >= 6
  // Test static assert
  // Uncomment next line to test inconsistent product

  // std::cout << "M1 * M1 = " << std::endl << M1 * M1 << std::endl;
#endif
  
#if QUESTION >= 7
  // Test class template specialization
  {
    Matrix<1,10> small_matrix;
    Matrix<100000,1> large_matrix;
    std::cout << "Small matrix is on " << (small_matrix.on_stack ? "stack": "heap") << std::endl;
    std::cout << "Large matrix is on " << (large_matrix.on_stack ? "stack": "heap") << std::endl << std::endl;

    Matrix<100000,10> product = large_matrix * small_matrix;
    std::cout << "Product is on " << (product.on_stack ? "stack": "heap") << std::endl << std::endl;
  }
#endif

#if QUESTION >= 8
  // Test concepts
  {
    // Uncomment next line to test concept checking
    
    // Matrix<3,2,std::string> M;
    
    // Also test on complex matrices
    Matrix<2,2,std::complex<double>> Mc { {1., {0.,1.}}, {{0.,-1.}, 1.} };
    Matrix<2,1,std::complex<double>> Vc { {{0.,1.}}, {-1.} };
    std::cout << "Mc = " << std::endl << Mc << std::endl;
    std::cout << "Vc = " << std::endl << Vc << std::endl;
    std::cout << "Mc * Vc = " << std::endl << Mc * Vc << std::endl;
  }
#endif

  return 0;
}
