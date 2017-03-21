# Morpheus
A Linear Algebra library in C++

                                                                                
This library is being written to serve two purposes:                            
1. To learn C++ and aid my learning of Linear Algebra                           
3. To have my own library that I can use for future projects                    
                                                                                
Features of Morpheus
--------------------

The library currently has the capability to:

1. Add and Subtract two matrices via operators +, += , - and -=
2. Multiply two matrices via operators * and *=
3. Transpose a matrix ( TBD )
4. Dot Product                 
5. Reduced Echelon form

Upcoming features
                                                                                
1. SVD and QR decomposition
3. Inverse of matrix
4. Compute rank of matrix
5. Implement Strassen's algorithm for multiplication

Requisites
-----------

1. a C++ compiler, supporting C++11 standard
2. the library code
3. GNU make utility

By default, the Makefile is written to use g++ as the compiler for the library.
In case, you prefer a different compiler, please make the required change in 
the Makefile
 
How to build
-------------

$> make library
$> make install

The above commands would make the shared library libmorpheus.so and 
install it in /usr/local/lib                                                    
