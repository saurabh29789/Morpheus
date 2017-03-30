
/*
 * Implementation of overloaded operators for matrix operations
 * Author : Saurabh Manchanda
 */
#include <morpheus.h>
#include <algorithm>
#include <stdexcept>
#include <string>

namespace Morpheus {

using std::string;

Matrix Matrix::operator+(const Matrix &x)
{
   Matrix result (rows, cols);
   int i = 0, j = 0;

   if (rows != x.rows || cols != x.cols) {
      string s = "Op+ Matrix dimensions don't match ";
      s += "[" + (std::to_string(rows)) + "x" +  (std::to_string(cols)) + "]";
      s += " vs ";
      s += "[" +  (std::to_string(x.rows))+ "x" +  (std::to_string(x.cols));
      throw std::invalid_argument(s);
   }

   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
         result.matrix[i][j] = matrix[i][j] + x.matrix[i][j];
      }
   }

   return result;
}

Matrix& Matrix::operator+=(const Matrix &x)
{
   *this = *this + x;
   return *this;
}

Matrix Matrix::operator-(const Matrix &x)
{
   Matrix result (rows, cols);
   int i = 0, j = 0;

   if (this->rows != x.rows || this->cols != x.cols) {
      string s = "Op- Matrix dimensions don't match ";
      s += "[" + (std::to_string(rows)) + "x" +  (std::to_string(cols)) + "]";
      s += " vs ";
      s += "[" +  (std::to_string(x.rows))+ "x" +  (std::to_string(x.cols));
      throw std::invalid_argument(s);
   }

   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
         result.matrix[i][j] = matrix[i][j] - x.matrix[i][j];
      }
   }

   return result;
}

Matrix& Matrix::operator-=(const Matrix &x)
{
   *this = *this - x;
   return *this;
}

Matrix Matrix::operator*(const Matrix &x)
{
   if (this->cols != x.rows) {
      string s = "Op* Matrix dimensions don't match ";
      s += "[" + (std::to_string(rows)) + "x" +  (std::to_string(cols)) + "]";
      s += " vs ";
      s += "[" +  (std::to_string(x.rows))+ "x" +  (std::to_string(x.cols));
      throw std::invalid_argument(s);
   }

#if 0
   /* for small matrices -> choose multiple_basic */
   if (this->cols < 2 || this->rows < 2 || x.cols < 2 || x.rows < 2)
      return this->int_mult_basic(x);
   else
      return this->int_mult_strassens(x);
#endif

   return this->int_mult_basic(x);
}

Matrix& Matrix::operator*=(const Matrix &x)
{
   *this = *this * x;
   return *this;
}

Matrix Matrix::operator*(const float x)
{
   Matrix result(*this);

   for (auto &rowvec : result.matrix) {
      std::transform(rowvec.begin(), rowvec.end(), rowvec.begin(),
               [x](float a) {return a*x;});
   }
   return result;
}

Matrix& Matrix::operator*=(const float x)
{
   return (*this)= (*this) * x;
}

float Matrix::dot(const Matrix &x)
{
   if (this-> rows != 1 || x.cols != 1) {
      throw std::invalid_argument("Dot Product: "
         "Left matrix should be row vector, "
         "Right matrix should be col vector\n");
   }

   Matrix pr = (*this) * x;
   
   return pr.matrix[0][0];
}

Matrix Matrix::operator!()
{
   Matrix result(this->cols, this->rows);

   for (int i = 0; i < this->rows; i++) {
      for (int j = 0; j < this->cols; j++) {
         result.matrix[j][i] = this->matrix[i][j];
      }
   }

   return result;
}

bool Matrix::operator==(const Matrix &rhs)
{
   if ((this->rows !=  rhs.rows) || (this->cols != rhs.cols) )
      return false;

   for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
         if(! is_float_same(this->matrix[i][j], rhs.matrix[i][j])) {
#ifdef MORPHEUS_DEBUG
            std::cout << i << ", " << j << " : " 
                      << this->matrix[i][j] << ", " << rhs.matrix[i][j] << "\n";
#endif
            return false;
         }

   return true;
}

bool Matrix::operator!=(const Matrix &rhs)
{
   return ! ((*this) == rhs);
}

}
