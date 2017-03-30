/*
 * Implementation for constructors and the ostream<< operator implementation
 * Author : Saurabh Manchanda
 */
#include <iostream>
#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <limits>
#include <cmath>

#include <morpheus.h>

namespace Morpheus {

using std::vector;
using std::initializer_list;

Matrix::Matrix(const int r, const int c)
   : matrix(r, std::vector<float>(c))
   , rows(r)
   , cols(c)
   , is_square(c == r)
{
}

Matrix::Matrix(const initializer_list<vector<float>> &data)
   : matrix(data)
   , rows(data.size())
   , cols(data.begin()->size())
   , is_square(rows == cols)
{
   size_t c = cols;
   /* Make sure all rows are of the same size */
   for (auto v : data) {
      if (c != (size_t)v.size()) {
         throw std::invalid_argument( "All rows should be of same length\n");
      }
   }
}

Matrix::Matrix(const vector<vector<float>> &data)
   : matrix(data)
   , rows(data.size())
   , cols(data.begin()->size())
   , is_square(rows == cols)
{
   size_t c = cols;
   /* Make sure all rows are of the same size */
   for (auto v : data) {
      if (c != (size_t)v.size()) {
         throw std::invalid_argument( "All rows should be of same length\n");
      }
   }
}

Matrix::Matrix(const Matrix& x)
   : matrix(x.matrix)
   , rows(x.rows)
   , cols(x.cols)
   , is_square(x.is_square)
{
}

std::ostream& operator<<(std::ostream& os, const Matrix &m)
{
   int i, j;
   for (i = 0; i < m.rows; i++) {
      if (0 == i) os << "|| ";
      else        os << " | ";

      for (j = 0; j < m.cols; j++) {
         float v = m.matrix[i][j];
         if ((0 == v) || Matrix::is_float_same(v, 0)) {
            os << 0 << " ";
         } else {
            os << v << " ";
         }
      }
      os << '\n';
   }

   return os;
}

// Note : Need to decide which comparison method to choose ?
bool Matrix::is_float_same(float a, float b)
{
/* Method 1 : Fractional difference */
   float diff  = std::fabs(a - b);
   float eps   = std::numeric_limits<float>::epsilon();
   float a_abs = std::fabs(a);
   float b_abs = std::fabs(b);
   float large = a_abs > b_abs ? a_abs : b_abs;

   return diff <= (large * eps);

/* Method 2 : by Knuth 
   return (std::abs(a-b) <= (std::abs(a) * eps));
*/

/* Method 3 : max ULP distance 
   typedef union {
      uint32_t i;
      float    f;
   } comp_s;
   int ULP = 10;
   comp_s a_x;
   comp_s b_x;
   
   a_x.f = a;
   b_x.f = b;
   if ( (a < 0) ^ (b < 0) ) return false;
  
   return ( std::abs(a_x.i - b_x.i) < ULP );

   float abs_v = std::fabs(a-b);
   float abs_a = std::fabs(a);
   float abs_b = std::fabs(b);
*/

/* Method 4 : essentially Equal : Again, by Knuth
   return (abs_v <= (((abs_a > abs_b) ? abs_b : abs_a) * eps) );
*/
}

bool Matrix::is_float_less(float a, float b)
{
   float eps = std::numeric_limits<float>::epsilon();

   return std::fabs(a-b) <= (100*eps);
}

}
