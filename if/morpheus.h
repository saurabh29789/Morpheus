/*
 * Header file containing the Morpheus class
 * Author : Saurabh Manchanda
 */

#ifndef __MORPHEUS_H__
#define __MORPHEUS_H__

/* Library for Linear Algebra operations for Integers */

#include <vector>
#include <iostream>
#include <initializer_list>

using std::vector;
using std::initializer_list;

class Morpheus
{
	public:
		Morpheus(const int, const int);
		Morpheus(const vector<vector<float>> &data);
		Morpheus(const initializer_list<vector<float>> &data);
		Morpheus(const Morpheus&);

		Morpheus operator+(Morpheus &);
		Morpheus& operator+=(Morpheus &);

		Morpheus operator-(Morpheus &);
		Morpheus& operator-=(Morpheus &);

		Morpheus operator*(Morpheus &);
		Morpheus& operator*=(Morpheus &);
		Morpheus operator*(float x);
		Morpheus& operator*=(float x);

		int dot(Morpheus &);
		Morpheus reduced_form();
		Morpheus& fill_random();

		friend std::ostream& operator<<(std::ostream&, const Morpheus &);

	private:
		Morpheus get_submatrix(int r1, int c1, int r2, int c2);

		Morpheus mult_basic(Morpheus &x);
		Morpheus mult_strassens(Morpheus &x);

	private:
		std::vector< std::vector <float> > matrix;
		int rows, cols;
		bool is_square;
};

#endif
