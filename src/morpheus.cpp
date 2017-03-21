/*
 * Implementation for constructors and the ostream<< operator implementation
 * Author : Saurabh Manchanda
 */
#include <iostream>
#include <initializer_list>
#include <vector>
#include <stdexcept>

#include <morpheus.h>

using std::vector;
using std::initializer_list;

Morpheus::Morpheus(const int r, const int c)
	: matrix(r, std::vector<float>(c))
	, rows(r)
	, cols(c)
	, is_square(c == r)
{
}

Morpheus::Morpheus(const initializer_list<vector<float>> &data)
	: matrix(data)
	, rows(data.size())
	, cols(data.begin()->size())
	, is_square(rows == cols)
{
	size_t c = cols;
	/* Make sure all rows are of the same size */
	for (auto v : data) {
		if ( c != (size_t)v.size()) {
			throw std::invalid_argument( "All rows should be of same length\n");
		}
	}
}

Morpheus::Morpheus(const vector<vector<float>> &data)
	: matrix(data)
	, rows(data.size())
	, cols(data.begin()->size())
	, is_square(rows == cols)
{
	size_t c = cols;
	/* Make sure all rows are of the same size */
	for (auto v : data) {
		if ( c != (size_t)v.size()) {
			throw std::invalid_argument( "All rows should be of same length\n");
		}
	}
}

Morpheus::Morpheus(const Morpheus& x)
	: matrix(x.matrix)
	, rows(x.rows)
	, cols(x.cols)
	, is_square(x.is_square)
{
}

std::ostream& operator<<(std::ostream& os, const Morpheus &m)
{
	int i, j;
	for (i = 0; i < m.rows; i++) {
		for (j = 0; j < m.cols; j++)
			if (m.matrix[i][j]) 
				os << m.matrix[i][j] << " ";
			else
				os << 0 << " ";
		os << '\n';
	}
	return os;
}
