/* 
 * Implementation of non-operator functions
 * Author : Saurabh Manchanda
 */
#include <utility>
#include <stdexcept>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include <morpheus.h>

namespace Morpheus {

using std::pair;
using std::vector;

static Matrix MORPHEUS_NULL_MATRIX(0,0);

Matrix Matrix::get_submatrix(int r1, int c1, int r2, int c2)
{
   if (r1 < 0 || r2 < 0 || c1 < 0 || c2 < 0) {
      throw std::out_of_range("Negative indices\n");
   }

   if (r1 >= rows || r2 >= rows) {
      throw std::out_of_range("request row > rows\n");
   }

   if (c1 >= cols || c2 >= cols) {
      throw std::out_of_range("request col > cols\n");
   }

   if (r2 < r1 || c2 < c1) {
      throw std::out_of_range(
            "(r2,c2) should not be to the left of (r1, c1)\n");
   }

   Matrix result(r2 - r1 + 1, c2 - c1 + 1);

   for (; r1 <= r2; r1++) {
      for (int iter_c = c1; iter_c <= c2; iter_c++) {
         result.matrix[r1][iter_c] = matrix[r1][iter_c];
      }
   }

   return result;
}

void Matrix::fill_random(int min, int max)
{
   std::random_device r;
   std::default_random_engine el(r());
   std::uniform_int_distribution<int> uniform_dist(min, max);
   std::srand(std::time(NULL));

   for (auto &row_vec : this->matrix) {
      std::transform(row_vec.begin(), row_vec.end(), row_vec.begin(),
            [&](int a) { return uniform_dist(el);});
   }
}

Matrix Matrix::eye(int rows)
{
   return make_identity(rows, rows);
}

pMM Matrix::int_get_partial_inverse()
{
   elim_t eltype = MORPHEUS_GAUSS_PARTINV;
   pair <pMM, pMM> gauss_out = this->int_gaussian_elimination(eltype);

   return gauss_out.first;
}

/* 
 * Use Gauss-Jordan elimination method
 * Step 1: A -> K, U, P : partialInverse, U of LDU, and Permutation matrix
 * Step 2: Reduce U to I, performing same ops on K
 * Step 3: inv(A) = K * P
 * The Step 3 is done as part of Step 1. So, it's not reqd now.
 * http://stackoverflow.com/questions/17010565/finding-matrix-inverse-by-gaussian-elimination-with-partial-pivoting#17012096
 */
Matrix Matrix::inverse()
{
   if (! is_square) {
      throw std::length_error("Non-square matrices can't be inverted\n");
   }

   /* [first::K   second::U] */
   pMM gauss_out = this->int_get_partial_inverse();

   Matrix& mat_K = (gauss_out.first);
   Matrix& mat_U = (gauss_out.second);

#ifdef MORPHEUS_DEBUG
   std::cout << "inverse: after gaussian elimination\n";
   std::cout << "Partial Inverse\n" << mat_K;
   std::cout << "Reduced A      \n" << mat_U;
#endif

   /*
    * If the rank is not the same as rows/cols, matrix is singular
    * Checked by making sure all pivots are non-zero
    */
   for (int i = 0; i < rows; i++) {
      if (0 == mat_U.matrix[i][i]) {
         throw std::domain_error("Matrix is singular\n");
      }
   }

   /*
    * Matrix is non-singular.
    * Keep going until mat_U is an Identity matrix
    * After the above reduction to 'U', reduce the mat_U, to I
    * NOTE: pivot_el should be '1' in each case as int_gaussian_elimination
    *       would have divided the U by diagonl matrix, D
    */

   for (int c = cols - 1; c >= 0; c--) {
      int pivot_row = c;
      float pivot_el = mat_U.matrix[pivot_row][c];

      for (int j = 0; j < pivot_row; j++) {
         float mult = mat_U.matrix[j][c] / pivot_el;

         /*
          * For 'U' matrix, only cell {c,c} has data;
          * For the partial inverse matrix, other cells of row c
          * can have non-zero data and they must take part in the elimination
          */
         mat_U.matrix[j][c] -= pivot_el * mult;

         for (int k = 0; k < cols; k++) {
            mat_K.matrix[j][k] -= mat_K.matrix[pivot_row][k] * mult;
         }
      }
   }

   return mat_K;
}

Matrix Matrix::make_identity(int r, int c)
{
   Matrix M(r, c);
   int min_val = std::min(r,c);

   for (int i = 0; i < min_val; i++) {
      M.matrix[i][i] = 1;
   }

   return M;
}

pair <pMM, Matrix> Matrix::lu_decomposition()
{
   /* [L | U] [P | null_matrix] */
   pair <pMM, pMM> k = this->int_gaussian_elimination(MORPHEUS_GAUSS_LU);
   return std::make_pair(k.first, k.second.first);
}

pair <pMM, pMM> Matrix::ldu_decomposition()
{
   return this->int_gaussian_elimination(MORPHEUS_GAUSS_LDU);
}

Matrix Matrix::rref()
{
   /* Start with the U part of LDU decomposition */
   Matrix rref_m = (this->ldu_decomposition()).first.second;

   /* Iterate the rows in reverse.
    * Foreach row, find the first non-zero value, (1)
    * And use it to zero out everything above it.
    */

#ifdef MORPHEUS_DEBUG
   std::cout << "START WITH \n" << rref_m;
#endif
   for (int rnum = rows - 1; rnum >= 0; rnum--) {

      int pivot_col;
      float pivot_val;
      for (pivot_col = 0; pivot_col < cols; pivot_col++) {
         pivot_val = rref_m.matrix[rnum][pivot_col];
         if (0 != pivot_val) break;
      }
      if (pivot_col == cols) continue;

#ifdef MORPHEUS_DEBUG
      std::cout << "  Found value at " << rnum << " at col " << pivot_col << "\n";
#endif

      //if ( (*rowvec)[j] != 0 : THROW EXCEPTION

      for (int rnum_in = 0; rnum_in < rnum; rnum_in++) {
         int col_iter;
         float mult, val;
         val = rref_m.matrix[rnum_in][pivot_col];

#ifdef MORPHEUS_DEBUG
         std::cout <<"      reducing at " << rnum_in << " " << val << "\n";
#endif
         if (0 == rref_m.matrix[rnum_in][pivot_col]) continue;

         mult = val / pivot_val;
         for (col_iter = pivot_col; col_iter < cols; col_iter++) {
            float prod = rref_m.matrix[rnum][col_iter] * mult;
            rref_m.matrix[rnum_in][col_iter] -= prod;
         }
      }
   }

   return rref_m;
}

/* NOTE: Should this be a friend function instead ? */
int Matrix::find_switch_row(Matrix &mat_U,
                 vector<float> &scaled_factors,
                 int pivot)
{
   int switch_row = -1;
   float max_val = 0;

   for (int i = pivot + 1; i < mat_U.rows; i++) {
      if (is_float_same(0, scaled_factors[i])) continue;

      float val = std::fabs(mat_U.matrix[i][pivot]);
            val /= scaled_factors[i];

      if ((-1 == switch_row) || (val > max_val)) {
         switch_row = i;
         max_val = val;
         continue;
      }
   }

   return switch_row;
}

/*
 * Output
 * 1) < <L, U>, P, D>
 * 2) < <L, U>, P, Null>
 * 3) < <Pi, U>, P, Null>
 */
pair <pMM, pMM> Matrix::int_gaussian_elimination(const elim_t type)
{
   int pivot = 0;
   const int is_L_required = (MORPHEUS_GAUSS_LDU == type || 
                              MORPHEUS_GAUSS_LU  == type);

   const int is_D_required = (MORPHEUS_GAUSS_LDU     == type ||
                              MORPHEUS_GAUSS_PARTINV == type);

   const int is_I_required = (MORPHEUS_GAUSS_PARTINV == type);

   Matrix mat_L_I  = (is_L_required || is_I_required) ?
                           Matrix::make_identity(rows, rows) :
                           MORPHEUS_NULL_MATRIX;

   Matrix mat_D    = is_D_required ?
                           Matrix::make_identity(rows, cols) :
                           MORPHEUS_NULL_MATRIX;
   Matrix mat_Perm = Matrix::make_identity(rows, rows);
   Matrix mat_U (*this);

   vector<float> scaled_factors; /* For Scaled Pivoting */

   int num_switches = 0;

   for (auto &v : mat_U.matrix) {
      float max = v[0];
      for (auto &k : v) {
         if (std::fabs(k) > max) max = std::fabs(k);
      }
      scaled_factors.push_back(max);
   }

   while (pivot < mat_U.rows && pivot < mat_U.cols) {

      // TODO : MOVE THIS TO A SEPARATE FUNCTION
      /*
       * Is pivot_col for current pivot_row a small value |x| < 0.01 ?
       * If yes, use Scaled Partial Pivoting to find the best row to swap with
       */
      if (std::fabs(mat_U.matrix[pivot][pivot]) < 0.01) {
         int switch_row = find_switch_row(mat_U, scaled_factors, pivot);

         if (-1 != switch_row) {
            std::swap(mat_U.matrix[pivot],
                      mat_U.matrix[switch_row]);

            std::swap(mat_Perm.matrix[pivot],
                      mat_Perm.matrix[switch_row]);

            /*
             * We can remove this, but then
             * we would need to multiply it with mat_Perm later
             */
            if (MORPHEUS_GAUSS_PARTINV == type) {
               std::swap(mat_L_I.matrix[pivot],
                         mat_L_I.matrix[switch_row]);
            } else {
               /* Swap the subdiagonal entries of the two rows */
               for (int k = 0; k < pivot; k++) {
                  std::swap(mat_L_I.matrix[pivot][k],
                            mat_L_I.matrix[switch_row][k]);
               }
            }
#ifdef MORPHEUS_DEBUG
            std::cout << "int_gaussian_elimination:"
                      << " Swapped rows " << pivot
                      << " with "         << switch_row << "\n";
#endif
            num_switches++;
         }
      }

      /* Here, we get the diagonal matrix 'D' from LDU */
      /* Dividing row by pivot element to set pivot el = 1 */
      float pivot_el = mat_U.matrix[pivot][pivot];
      if (is_D_required) {
         mat_D.matrix[pivot][pivot] = pivot_el;

         std::transform(
               mat_U.matrix[pivot].begin(),
               mat_U.matrix[pivot].end(),
               mat_U.matrix[pivot].begin(),
               [pivot_el](float cur_val)
                  { return (pivot_el ? (cur_val/pivot_el) : cur_val); }
            );

         if (is_I_required) {
            std::transform(
                  mat_L_I.matrix[pivot].begin(),
                  mat_L_I.matrix[pivot].end(),
                  mat_L_I.matrix[pivot].begin(),
                  [pivot_el](float cur_val)
                     { return pivot_el ? (cur_val/pivot_el) : cur_val; }
            );
         }

         pivot_el = mat_U.matrix[pivot][pivot];
      }

      if (!pivot_el) std::cout << "\nPIVOT IS ZERO HERE\n";
      /* Start reducing rows following pivot */
      for (int i = pivot + 1; pivot_el && i < mat_U.rows; i++) {
         float el_r = mat_U.matrix[i][pivot];
         float mult = el_r/pivot_el;

#ifdef MORPHEUS_DEBUG
         std::cout << "int_gaussian_elimination: "
                   << "  pivot row " << pivot
                   << "; pivot el  " << pivot_el
                   << "; el_r      " << el_r
                   << "; mult      " << mult << '\n';
#endif

         if ( is_L_required) {
            mat_L_I.matrix[i][pivot] = mult;
         }

      /* Operating on mat_U and mat_L_I in separate loops for efficiency */
         for (int j = 0; j < mat_U.cols; j++) {

            float minuend    = mat_U.matrix[i][j];
            float subtrahend = mat_U.matrix[pivot][j] * mult;
            bool is_same = is_float_same (minuend, subtrahend);

            if (is_same) mat_U.matrix[i][j] = 0;
            else         mat_U.matrix[i][j] -= subtrahend;
         }

         if (is_I_required) {
            for (int j = 0; j < mat_U.cols; j++) {

               float minuend    = mat_L_I.matrix[i][j];
               float subtrahend = mat_L_I.matrix[pivot][j] * mult;
               bool is_same = is_float_same (minuend, subtrahend);

               if (is_same) mat_L_I.matrix[i][j] = 0;
               else         mat_L_I.matrix[i][j] -= subtrahend;
            }
         }

#ifdef MORPHEUS_DEBUG
         std::cout << "int_gaussian_elimination: mat_L \n" << mat_L_I;
         std::cout << "int_gaussian_elimination: mat_U \n" << mat_U;;
#endif
      }

      pivot++;
   }

   return std::make_pair(std::make_pair(mat_L_I, mat_U),
                         std::make_pair(mat_Perm, mat_D) );
}


/* NOTE : For larger matrix, a trick to optimize would be to take the
 *        transform of 'x' and then do the multiplication of rows with rows
 *        This improves the access of elements of 'x'
 */
Matrix Matrix::int_mult_basic(const Matrix &x)
{
   Matrix result(this->rows, x.cols);
   int i = 0, j = 0, k = 0;

   for (i = 0; i < rows; i++) {
      for (j = 0; j < x.cols; j++) {

         result.matrix[i][j] = 0;
         for (k = 0; k < x.rows; k++) {
            float addend_1 = result.matrix[i][j];
            float addend_2 = this->matrix[i][k]*x.matrix[k][j];

            if (is_float_same( - addend_1, addend_2) ) {
               result.matrix[i][j] = 0;
            } else {
               result.matrix[i][j] += addend_2;
            }
         }
      }
   }

   return result;
}

Matrix Matrix::int_mult_strassens(const Matrix &x)
{
#if 0
   Matrix A11 = this->get_submatrix(0, 0, this->rows/2, this->cols/2);
   Matrix A12 = this->get_submatrix(0, this->cols/2 + 1,  this->rows/2, this->cols - 1);
   Matrix A21 = this->get_submatrix(this->rows/2 + 1, 0, this->rows - 1, this->cols/2);
   Matrix A22 = this->get_submatrix(this->rows/2 + 1, this->cols/2 + 1, this->rows - 1, this->cols - 1);

   Matrix B11 = x.get_submatrix(0, 0, x.rows/2, x.cols/2);
   Matrix B12 = x.get_submatrix(0, x.cols/2 + 1,  x.rows/2, x.cols - 1);
   Matrix B21 = x.get_submatrix(x.rows/2 + 1, 0, x.rows - 1, x.cols/2);
   Matrix B22 = x.get_submatrix(x.rows/2 + 1, x.cols/2 + 1, x.rows - 1, x.cols - 1);

   Matrix M1 = (A11 + A22) * (B11 + B22);
   Matrix M2 = (A21 + A22) * B11;
   Matrix M3 = A11 * ( B12 - B22 );
   Matrix M4 = A22 * ( B21 - B11 );
   Matrix M5 = (A11 + A12) * B22;
   Matrix M6 = (A21 - A11) * (B11 + B12);
   Matrix M7 = (A12 - A22) * (B21 * B22);

   Matrix C11 = M1 + M4 - M5 + M7;
   Matrix C12 = M3 + M5;
   Matrix C21 = M2 + M4;
   Matrix C22 = M1 - M2 + M3 + M6;
#endif

   return *this;

}

}
