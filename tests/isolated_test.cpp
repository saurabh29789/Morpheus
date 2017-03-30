/*
 * Source code for isolated tests for Matrix
 * Currently, the test only make sure that the operator invocations don't
 * result in any error or crash.
 * The correctness of the implementation is not verified yet.
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include "morpheus.h"

using namespace Morpheus;

enum test_types
{
   MORPHEUS_TEST_ADD = (1 << 0),
   MORPHEUS_TEST_SUB = (1 << 1),
   MORPHEUS_TEST_MUL = (1 << 2),
   MORPHEUS_TEST_DOT = (1 << 3),
   MORPHEUS_TEST_LU  = (1 << 4),
   MORPHEUS_TEST_INV = (1 << 5),
   MORPHEUS_TEST_RREF = (1 << 6),
   MORPHEUS_TEST_ALL = ~0,
};

int test_add(int range_min, int range_max)
{
   std::cout << "-- Begin Add tests --\n";

   for (int i = range_min; i < range_max; i++) {
      for (int j = range_min; j < range_max; j++) {
         Matrix mat_1(i, j), mat_2(i, j);

         mat_1.fill_random();
         mat_2.fill_random();

         std::cout << "Matrix 1\n" << mat_1;
         std::cout << "\nMatrix 2\n" << mat_2;
         std::cout << "\nSum  \n" << mat_1 + mat_2;
         std::cout << "\n________________\n";
      }
   }

   return 0;
}

int test_sub(int range_min, int range_max)
{
   std::cout << "-- Begin Subtraction tests --\n";

   for (int i = range_min; i < range_max; i++) {
      for (int j = range_min; j < range_max; j++) {
         Matrix mat_1(i, j), mat_2(i, j);

         mat_1.fill_random();
         mat_2.fill_random();

         std::cout << "Matrix 1\n" << mat_1;
         std::cout << "\nMatrix 2\n" << mat_2;
         std::cout << "\nDifference \n" << mat_1 - mat_2;
         std::cout << "\n________________\n";
      }
   }
   return 0;
}

int test_mul(int range_min, int range_max)
{
   std::cout << "-- Begin Multiplication tests --\n";

   for (int i = range_min; i < range_max; i++) {
      for (int j = range_min; j < range_max; j++) {
         for (int k = range_min; k < range_max; k++) {
            Matrix mat_1(i, j), mat_2(j, k);

            mat_1.fill_random();
            mat_2.fill_random();

            std::cout << "Matrix 1\n" << mat_1;
            std::cout << "\nMatrix 2\n" << mat_2;
            std::cout << "\nProduct \n" << mat_1 * mat_2;
            std::cout << "\n________________\n";
         }
      }
   }
   return 0;
}

int test_dot(int range_min, int range_max)
{
   std::cout << "-- Begin Dot Product tests --\n";

   for (int i = range_min; i < range_max; i++) {
      Matrix mat_1(1, i), mat_2(i, 1);

      mat_1.fill_random();
      mat_2.fill_random();

      std::cout << "Matrix 1\n" << mat_1;
      std::cout << "\nMatrix 2\n" << mat_2;
      std::cout << "\nDot Product  \n" << mat_1.dot(mat_2);
      std::cout << "\n________________\n";
   }
   return 0;
}

int test_LUdecomposition(int range_min, int range_max)
{
   std::cout << "-- Begin LU decomposition tests --\n";

   for (int i = range_min; i < range_max; i++) {
      for (int j = range_min; j < range_max; j++) {
         Matrix mat_1(i,j);
         mat_1.fill_random();
         std::pair<pMM, Matrix> p = mat_1.lu_decomposition();
         Matrix &L = p.first.first;
         Matrix &U = p.first.second;
         Matrix &P = p.second;

         std::cout << "Matrix\n" << mat_1;
         std::cout << "\nL  \n" << L;
         std::cout << "\nU  \n" << U;
         std::cout << "\nP  \n" << P;
         std::cout <<" \nLU \n" << L * U;
         std::cout <<" \nPA \n" << P * mat_1;

         std::cout << "\n________________\n";
      }
   }
   return 0;
}

int test_inverse(int range_min, int range_max)
{
   std::cout << "-- Begin Matrix inverse tests --\n";

   for (int i = range_min; i < range_max; i++) {
         Matrix mat_1(i, i);

      try {
         mat_1.fill_random();
         Matrix mat_2 = mat_1.inverse();
         Matrix prod = mat_1 * mat_2;

         std::cout << "Matrix\n" << mat_1;
         std::cout << "\nInverse \n" << mat_2;
         std::cout << "\nProduct \n" << prod;

         std::cout << "\n________________\n";
      } catch (std::exception & e) {
         std::cout << e.what();
         std::cout << mat_1;
      }
   }
   return 0;
}

int test_rref(int range_min, int range_max)
{
   std::cout << "-- Begin Matrix RREF tests --\n";

   for (int i = range_min; i < range_max; i++) {
      for (int j = range_min; j < range_max; j++) {
         Matrix mat_1(i, j);

      try {
         mat_1.fill_random();
         Matrix mat_2 = mat_1.rref();

         std::cout << "Matrix\n" << mat_1;
         std::cout << "\nRREF \n" << mat_2;
         std::cout << "\n________________\n";
      } catch (std::exception & e) {
         std::cout << e.what();
         std::cout << mat_1;
      }
      }
   }
   return 0;
}

const int default_range_min = 1;
const int default_range_max = 4;

void usage(const char *f)
{
   std::cerr << "Usage : \n" 
             << f << " -a -s -m -d -l -i -B <val> -E <val>\n"
             << "-a       : Run addition tests\n"
             << "-s       : Run subtraction tests\n"
             << "-m       : Run multiplication tests\n"
             << "-d       : Run dot-product tests\n"
             << "-l       : Run LU decomposition tests\n"
             << "-i       : Run Inverse tests\n"
             << "-r       : Run Reduced Row Echelon Form test\n"
             << "-B <val> : Minimum matrix dimension [default : 1]\n"
             << "-E <val> : Maximum matrix dimension [default : 4]\n";
}

int get_args(int argc, char *argv[], int &r_min, int &r_max, unsigned int *t)
{
   int opt;

   r_min = default_range_min;
   r_max = default_range_max;
   int show_help = 0;

   while ( -1 != (opt = getopt(argc, argv, "AasmdlirB:E:h")) )
   {
      switch(opt) {
         case 'A' : *t |= MORPHEUS_TEST_ALL; break;
         case 'a' : *t |= MORPHEUS_TEST_ADD; break;
         case 's' : *t |= MORPHEUS_TEST_SUB; break;
         case 'm' : *t |= MORPHEUS_TEST_MUL; break;
         case 'd' : *t |= MORPHEUS_TEST_DOT; break;
         case 'l' : *t |= MORPHEUS_TEST_LU ; break;
         case 'i' : *t |= MORPHEUS_TEST_INV; break;
         case 'r' : *t |= MORPHEUS_TEST_RREF; break;
         case 'B' : r_min = std::stoi(optarg); break;
         case 'E' : r_max = std::stoi(optarg); break;
         case 'h' : show_help = 1; break;
      }
   }

   if (show_help) {
      usage(argv[0]);
      exit(0);
   }
   if (r_min >= r_max) {
      std::cerr << "r_min < r_max : "
                << "r_min (" << r_min << ") "
                << "r_max (" << r_max << ")\n";
      usage(argv[0]);
      return -1;
   }

   return 0;
}

int main(int argc, char *argv[])
{
   try {
      int range_min = 0;
      int range_max = 0;
      unsigned int tests_to_run = 0;

      if (0 != get_args(argc, argv, range_min, range_max, &tests_to_run))
         return -1;

      if (MORPHEUS_TEST_ADD & tests_to_run) test_add(range_min, range_max);
      if (MORPHEUS_TEST_SUB & tests_to_run) test_sub(range_min, range_max);
      if (MORPHEUS_TEST_MUL & tests_to_run) test_mul(range_min, range_max);
      if (MORPHEUS_TEST_DOT & tests_to_run) test_dot(range_min, range_max);
      if (MORPHEUS_TEST_LU  & tests_to_run) test_LUdecomposition(range_min, range_max);
      if (MORPHEUS_TEST_INV & tests_to_run) test_inverse(range_min, range_max);
      if (MORPHEUS_TEST_RREF & tests_to_run) test_rref(range_min, range_max);

   } catch(std::invalid_argument &ia) {
      std::cout << ia.what();
   }
}
