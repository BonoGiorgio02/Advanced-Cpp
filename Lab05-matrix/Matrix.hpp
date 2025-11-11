#pragma once
#include <iostream>
#include <array>
#include <initializer_list>
#include <exception>

namespace matrix {
  // Declare the class template here
  template<int n, int m, typename Scalar> class Matrix;

  template<int n, int m, typename Scalar>std::ostream& operator<<(std::ostream&, const Matrix<n, m, Scalar>&);

  template<int n, int m, typename Scalar = double> class Matrix {
    std::array<Scalar, n * m> data;

    // QUESTION 3
    Matrix<n, m, Scalar>& operator+=(const Matrix<n, m, Scalar>&);
    // END QUESTION 3

    public:
      using scalar_type = Scalar;
 
      // = Scalar() means give to val a default value that, in the case of int or double, it is 0 or 0.0
      // If Scalar is a class, it will call its default constructor
      Matrix(const Scalar val = Scalar());
      Matrix(const Matrix&) = default;
      Matrix(const std::initializer_list<std::initializer_list<Scalar>>&);
      Scalar& operator[](const std::pair<int, int>&);

      // QUESTION 2
      friend std::ostream& operator<<<n, m, Scalar>(std::ostream& os, const Matrix<n, m, Scalar>&);

      // QUESTION 3
      Matrix<n, m, Scalar> operator+(const Matrix<n, m, Scalar>&);

      // QUESTION 4
      template<int p, int q> Matrix<n, q, Scalar> operator*(const Matrix<p, q, Scalar>&);
      template<int, int, typename> friend class Matrix;
  };

  template<int n, int m, typename Scalar> Matrix<n, m, Scalar>::Matrix(const Scalar val) {
    for(Scalar& el : this->data) el = val;
    // this->data.fill(val); // I can do it also like this
  }

  template<int n, int m, typename Scalar> Matrix<n, m, Scalar>::Matrix(const std::initializer_list<std::initializer_list<Scalar>>& coefs) {
    if(coefs.size() != n) throw std::runtime_error("Init: bad number of rows");
    auto it = this->data.begin();
    for(auto lineCoefs : coefs){
      if(lineCoefs.size() != m) throw std::runtime_error("Init: bad number of columns");
      for(auto coef : lineCoefs)
        *(it++) = coef;
    }
  }

  template<int n, int m, typename Scalar> Scalar& Matrix<n, m, Scalar>::operator[](const std::pair<int, int>& coords) {
    return this->data[coords.first *m + coords.second];
  }

  // QUESTION 2
  template<int n, int m, typename Scalar>std::ostream& operator<<(std::ostream& os, const Matrix<n, m, Scalar>& mat) {
    int count = 0;
    os << "[";
    for(auto& el : mat.data){
      if(count % m == 0) os << '[';
      os << el;
      ++count;
      if(count % m != 0) os << ", ";
      else if (count != n * m) os << "],\n";
      else os << ']';
    }
    os << "]" << std::endl;
    return os;
  }

  // QUESTION 3
  template<int n, int m, typename Scalar>Matrix<n, m, Scalar>& Matrix<n, m, Scalar>::operator+=(const Matrix<n, m, Scalar>& mat) {
    auto src = mat.data.begin();
    for(auto it = this->data.begin(), end = this->data.end(); it != end; ++it, ++src) *it += *src;
    return *this;
  }

  // Writing in this way, we say that both matrices must be of dimension n, m so we have static checks by the compiler
  // so we don't have to check if the two matrices have the same dimension
  template<int n, int m, typename Scalar>Matrix<n, m, Scalar> Matrix<n, m, Scalar>::operator+(const Matrix<n, m, Scalar>& mat) {
    Matrix<n, m, Scalar> res(*this);
    res += mat;
    return res;
  }

  template<int n, int m, typename Scalar>
  template<int p, int q> Matrix<n, q, Scalar> Matrix<n, m, Scalar>::operator*(const Matrix<p, q, Scalar>& B) {
    if(m != p) throw std::runtime_error("Dimension mismatch between matrices");
    Matrix<n, q, Scalar> res;
    auto pr = res.data.begin();
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < q; ++j, ++pr) {
        auto p1 = this->data.begin() + i * m;
        auto p2 = B.data.begin() + j;
        for(int k = 0; k < p; ++k, ++p1, p2 += p)
          *pr += *p1 * *p2;
      }
    }
    return res;
  }
}