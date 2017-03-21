
/*
 * Implementation of overloaded operators for matrix operations
 * Author : Saurabh Manchanda
 */
#include <morpheus.h>
#include <algorithm>
#include <stdexcept>

Morpheus Morpheus::operator+(Morpheus &x)
{
	Morpheus result (rows, cols);
	int i = 0, j = 0;
	if ( rows != x.rows || cols != x.cols) {
		throw std::invalid_argument("Matrix dimensions don't match\n");
	}

	for ( i = 0; i < rows; i++ )
		for ( j = 0; j < cols; j++ )
			result.matrix[i][j] = matrix[i][j] + x.matrix[i][j];

	return result;
}

Morpheus& Morpheus::operator+=(Morpheus &x)
{
	*this = *this + x;
	return *this;
}

Morpheus Morpheus::operator-(Morpheus &x)
{
	Morpheus result (rows, cols);
	int i = 0, j = 0;
	if ( this->rows != x.rows || this->cols != x.cols) {
		throw std::invalid_argument("Matrix dimensions don't match\n");
	}

	for ( i = 0; i < rows; i++ )
		for ( j = 0; j < cols; j++ )
			result.matrix[i][j] = matrix[i][j] - x.matrix[i][j];

	return result;
}

Morpheus& Morpheus::operator-=(Morpheus &x)
{
	*this = *this - x;
	return *this;
}

Morpheus Morpheus::mult_basic(Morpheus &x)
{
	Morpheus result(this->rows, x.cols);
	int i = 0, j = 0, k = 0;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < x.cols; j++) {

			result.matrix[i][j] = 0;
			for (k = 0; k < x.rows; k++) {
				result.matrix[i][j] += this->matrix[i][k]*x.matrix[k][j];
			}
		}
	}
	return result;
}

Morpheus Morpheus::mult_strassens(Morpheus &x)
{
#if 0 
	Morpheus A11 = this->get_submatrix(0, 0, this->rows/2, this->cols/2);
	Morpheus A12 = this->get_submatrix(0, this->cols/2 + 1,  this->rows/2, this->cols - 1);
	Morpheus A21 = this->get_submatrix(this->rows/2 + 1, 0, this->rows - 1, this->cols/2);
	Morpheus A22 = this->get_submatrix(this->rows/2 + 1, this->cols/2 + 1, this->rows - 1, this->cols - 1);

	Morpheus B11 = x.get_submatrix(0, 0, x.rows/2, x.cols/2);
	Morpheus B12 = x.get_submatrix(0, x.cols/2 + 1,  x.rows/2, x.cols - 1);
	Morpheus B21 = x.get_submatrix(x.rows/2 + 1, 0, x.rows - 1, x.cols/2);
	Morpheus B22 = x.get_submatrix(x.rows/2 + 1, x.cols/2 + 1, x.rows - 1, x.cols - 1);

	Morpheus M1 = (A11 + A22) * (B11 + B22);
	Morpheus M2 = (A21 + A22) * B11;
	Morpheus M3 = A11 * ( B12 - B22 );
	Morpheus M4 = A22 * ( B21 - B11 );
	Morpheus M5 = (A11 + A12) * B22;
	Morpheus M6 = (A21 - A11) * (B11 + B12);
	Morpheus M7 = (A12 - A22) * (B21 * B22);

	Morpheus C11 = M1 + M4 - M5 + M7;
	Morpheus C12 = M3 + M5;
	Morpheus C21 = M2 + M4;
	Morpheus C22 = M1 - M2 + M3 + M6;
#endif
	return *this;

}

Morpheus Morpheus::operator*(Morpheus &x)
{
	if ( this->cols != x.rows ) {
		throw std::invalid_argument("Matrix dimensions don't match\n");
	}

#if 0
	/* for small matrices -> choose multiple_basic */
	if (this->cols < 2 || this->rows < 2 || x.cols < 2 || x.rows < 2)
		return this->mult_basic(x);
	else
		return this->mult_strassens(x);
#endif

	return this->mult_basic(x);
}

Morpheus& Morpheus::operator*=(Morpheus &x)
{
	*this = *this * x;
	return *this;
}

Morpheus Morpheus::operator*(float x)
{
	Morpheus result(*this);

	for (auto &rowvec : result.matrix) {
		std::transform(rowvec.begin(), rowvec.end(), rowvec.begin(),
					[x](float a) {return a*x;});
	}
	return result;
}

Morpheus& Morpheus::operator*=(float x)
{
	return (*this)= (*this) * x;
}

int Morpheus::dot(Morpheus &x)
{
	if (this-> rows != 1 || x.cols != 1) {
		throw std::invalid_argument("Dot Product: "
			"Left matrix should be row vector, "
			"Right matrix should be col vector\n");
	}

	Morpheus pr = (*this)*x;
	
	return pr.matrix[0][0];
}
