# Morpheus
A Linear Algebra library in C++


This library is being written to serve two purposes:
1. To learn C++ and aid my learning of Linear Algebra
3. To have my own library that I can use for future projects

#Features of Morpheus
--------------------

The library currently has the capability to:

1. Arithematic Operations ( +, - and * )
2. Transpose a matrix
3. Dot Product
4. Reduced Row Echelon Form
5. Inverse of matrix
6. LU and LDU decomposition

#Upcoming features

1. SVD and QR decomposition
2. Compute rank of matrix
3. Implement Strassen's algorithm for multiplication

#Requisites
-----------

1. a C++ compiler, supporting C++11 standard
2. the library code
3. GNU make utility

By default, the Makefile is written to use g++ as the compiler for the library.
In case, you prefer a different compiler, please make the required change in
the Makefile

#How to make
-------------

$> make library
$> make install

The above commands would build the shared library libmorpheus.so and 
install it in /usr/local/lib

