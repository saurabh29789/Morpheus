/*
 * Header file containing the Morpheus class
 * Author : Saurabh Manchanda
 */

#ifndef __MORPHEUS_H__
#define __MORPHEUS_H__

/* Library for Linear Algebra operations for Integers */

#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>

namespace Morpheus {

using std::pair;
using std::vector;
using std::initializer_list;

class Matrix;
typedef pair <Matrix, Matrix> pMM;

class Matrix
{
   public: /* Constructors */
     Matrix(const int r, const int c);
     Matrix(const vector<vector<float>> &data);
     Matrix(const initializer_list<vector<float>> &data);
     Matrix(const Matrix&);

   public: /* Matrix Operations */
     Matrix  operator+ (const Matrix &);
     Matrix& operator+=(const Matrix &);

     Matrix  operator- (const Matrix &);
     Matrix& operator-=(const Matrix &);

     Matrix  operator* (const Matrix &);
     Matrix& operator*=(const Matrix &);
     Matrix  operator* (const float x);
     Matrix& operator*=(const float x);

     bool    operator==(const Matrix &);
     bool    operator!=(const Matrix &);

     Matrix  operator! ();
     float dot(const Matrix &);

     Matrix        rref();
     Matrix        inverse();
     pair <pMM, Matrix> lu_decomposition();
     pair <pMM, pMM>    ldu_decomposition();

   public: /* Additional functions */
     void fill_random(int min = 0, int max = 10);
     friend std::ostream& operator<<(std::ostream&, const Matrix &);
     static Matrix eye(int rows);

   private: /* Internal implementations */
     enum elim_t {
       MORPHEUS_GAUSS_LDU,
       MORPHEUS_GAUSS_LU,
       MORPHEUS_GAUSS_PARTINV,
     };
     pMM int_get_partial_inverse();
     pair <pMM, pMM> int_gaussian_elimination(const elim_t);

     /* (r1, r2) -> (r2, c2) [ both vertices are inclusive ] */
     Matrix get_submatrix(int r1, int c1, int r2, int c2);

     Matrix int_mult_basic(const Matrix &x);
     Matrix int_mult_strassens(const Matrix &x);

   private: /* Additional functions : internal */
     static bool is_float_same(float a, float b);
     static bool is_float_less(float a, float b);
     static Matrix make_identity(int r, int c);
     int    find_switch_row(Matrix&, vector<float>&, int);

   private: /* Data elements */
     vector<vector <float> > matrix;
     int rows, cols;
     bool is_square;
};

}

#endif
