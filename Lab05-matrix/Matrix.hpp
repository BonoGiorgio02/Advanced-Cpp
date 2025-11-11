#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <initializer_list>
#include <exception>
#include <concepts>

namespace matrix {
  // Declare the class template here
  template<typename T> concept Field =
    std::default_initializable<T> &&
    std::copy_constructible<T> &&
    std::move_constructible<T> &&
    std::destructible<T> &&
    std::equality_comparable<T> &&
    requires(T a, T b) {                 
			{ a + b } -> std::convertible_to<T>;
			{ a - b } -> std::convertible_to<T>;
			{ a * b } -> std::convertible_to<T>;
			{ a / b } -> std::convertible_to<T>;
  };

  template<size_t n, size_t m, Field Scalar> class Matrix;

  template<size_t n, size_t m, Field Scalar>
  std::ostream& operator<<(std::ostream&, const Matrix<n, m, Scalar>&);

  // General definition of the template
  template<size_t n, size_t m, Field Scalar, bool stack> struct Storage;

  // Partial specialization of the template, before I write free parameters (template parameters) and then I write the values
  // to create a specific version of Storage template, the version with bool stack = true
  template<size_t n, size_t m, Field Scalar> struct Storage<n, m, Scalar, true> {
      std::array<Scalar, n * m> data;
      static const bool on_stack = true;
  };

  template<size_t n, size_t m, Field Scalar> struct Storage<n, m, Scalar, false> {
      std::vector<Scalar> data;
      static const bool on_stack = false;

      Storage() {
        data.resize(n * m);
      }
  };

  constexpr size_t limit_size_on_stack = 10000;
  constexpr bool best_on_stack(size_t n, size_t m) {
    return n * m < limit_size_on_stack;
  }

  template<size_t n, size_t m, Field Scalar = double> class Matrix : public Storage<n, m, Scalar, best_on_stack(n, m)>{
    // QUESTION 3
    Matrix<n, m, Scalar>& operator+=(const Matrix<n, m, Scalar>&);
    // END QUESTION 3

    public:
      using scalar_type = Scalar;
 
      // = Scalar() means give to val a default value that, in the case of size_t or double, it is 0 or 0.0
      // If Scalar is a class, it will call its default constructor
      Matrix(const Scalar val = Scalar());
      Matrix(const Matrix&) = default;
      Matrix(const std::initializer_list<std::initializer_list<Scalar>>&);

      Scalar& operator[](const std::pair<size_t, size_t>&);

      // QUESTION 2
      friend std::ostream& operator<<<n, m, Scalar>(std::ostream& os, const Matrix<n, m, Scalar>&);

      // QUESTION 3
      Matrix<n, m, Scalar> operator+(const Matrix<n, m, Scalar>&);

      // QUESTION 4
      template<size_t p, size_t q> Matrix<n, q, Scalar> operator*(const Matrix<p, q, Scalar>&);
      template<size_t, size_t, Field> friend class Matrix;
  };

  template<size_t n, size_t m, Field Scalar> Matrix<n, m, Scalar>::Matrix(const Scalar val) {
    for(Scalar& el : this->data) el = val;
    // this->data.fill(val); // I can do it also like this
  }

  template<size_t n, size_t m, Field Scalar> Matrix<n, m, Scalar>::Matrix(const std::initializer_list<std::initializer_list<Scalar>>& coefs) {
    if(coefs.size() != n) throw std::runtime_error("Init: bad number of rows");
    auto it = this->data.begin();
    for(auto lineCoefs : coefs){
      if(lineCoefs.size() != m) throw std::runtime_error("Init: bad number of columns");
      for(auto coef : lineCoefs)
        *(it++) = coef;
    }
  }

  template<size_t n, size_t m, Field Scalar> Scalar& Matrix<n, m, Scalar>::operator[](const std::pair<size_t, size_t>& coords) {
    return this->data[coords.first *m + coords.second];
  }

  // QUESTION 2
  template<size_t n, size_t m, Field Scalar>std::ostream& operator<<(std::ostream& os, const Matrix<n, m, Scalar>& mat) {
    size_t count = 0;
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
  template<size_t n, size_t m, Field Scalar>Matrix<n, m, Scalar>& Matrix<n, m, Scalar>::operator+=(const Matrix<n, m, Scalar>& mat) {
    auto src = mat.data.begin();
    for(auto it = this->data.begin(), end = this->data.end(); it != end; ++it, ++src) *it += *src;
    return *this;
  }

  // Writing in this way, we say that both matrices must be of dimension n, m so we have static checks by the compiler
  // so we don't have to check if the two matrices have the same dimension
  template<size_t n, size_t m, Field Scalar>Matrix<n, m, Scalar> Matrix<n, m, Scalar>::operator+(const Matrix<n, m, Scalar>& mat) {
    Matrix<n, m, Scalar> res(*this);
    res += mat;
    return res;
  }

  template<size_t n, size_t m, Field Scalar>
  template<size_t p, size_t q> Matrix<n, q, Scalar> Matrix<n, m, Scalar>::operator*(const Matrix<p, q, Scalar>& B) {
    static_assert(m == p,"Dimension mismatch between matrices");
    Matrix<n, q, Scalar> res;
    auto pr = res.data.begin();
    if constexpr (m == 1) {
      std::cout << "Case 1 dimension matrix moltiplication" << std::endl;
      for(auto& p1 : this->data)
        for(auto& p2 : B.data) 
          *pr++ = p1 * p2;
    } 
    else {
      std::cout << "General matrix moltiplication" << std::endl;
      for(size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < q; ++j, ++pr) {
          auto p1 = this->data.begin() + i * m;
          auto p2 = B.data.begin() + j;
          for(size_t k = 0; k < p; ++k, ++p1, p2 += p)
            *pr += *p1 * *p2;
        }
      }
    }
    return res;
  }
}