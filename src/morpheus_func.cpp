/* 
 * Implementation of non-operator functions
 * Author : Saurabh Manchanda
 */
#include <stdexcept>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include <morpheus.h>

Morpheus Morpheus::get_submatrix(int r1, int c1, int r2, int c2)
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

	Morpheus result(r2 - r1 + 1, c2 - c1 + 1);
	for (; r1 <= r2; r1++)
		for (int iter_c = c1; iter_c <= c2; iter_c++)
			result.matrix[r1][iter_c] = matrix[r1][iter_c];

	return result;
}

/* Perform Gaussian Elimination to get 'U' */
Morpheus Morpheus::reduced_form()
{
	int pivot_col = 0;
	int pivot_row = 0;
	while ( pivot_row < this->rows && pivot_col < this->cols ) {
		
		/* Is pivot_col for current pivot_row a small value ? 
		 * If yes, switch it with a row of higher value from row > pivot_row
		 */
		if (matrix[pivot_row][pivot_col] < 1) {
			int i = pivot_row + 1;
			int switch_row = pivot_row;
			float max = matrix[pivot_row][pivot_col];
			for (; i < this->rows; i++) {
				if (matrix[i][pivot_col] > max) {
					max = matrix[i][pivot_col];
					switch_row = i;
				}
			}

			if (switch_row != pivot_row) {
				std::swap(matrix[pivot_row], matrix[switch_row]);
			}
		}

		float el = matrix[pivot_row][pivot_col];
		std::transform(
					matrix[pivot_row].begin(), matrix[pivot_row].end(),
					matrix[pivot_row].begin(),
					[el](float cur_val) {return el ? (cur_val/el) : cur_val;});
		el = matrix[pivot_row][pivot_col];

		/* Start reducing */
		for (int i = pivot_row + 1; i < this->rows; i++) {
			float el_r = matrix[i][pivot_col];
			float mult = el_r/el;
			for (int j = pivot_col; j < this->cols; j++) {
				matrix[i][j] -= matrix[pivot_row][j] * mult;

				/* deals with negative zeroes */
				if (0 == matrix[i][j]) matrix[i][j] = 0;
			}
		}

		pivot_row++;
		pivot_col++;
	}
	return *this;
}

Morpheus& Morpheus::fill_random()
{
	std::srand(std::time(NULL));
	
	for (auto &row_vec : this->matrix) {
		std::transform(row_vec.begin(), row_vec.end(), row_vec.begin(),
						[](int a) {return std::rand() % 20;});
	}
	return *this;
}
