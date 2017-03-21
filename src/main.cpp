#include <iostream>
#include <stdexcept>
#include "morpheus.h"

int main()
{
	try {
		Morpheus A_2x2 ({{1,2},{3, 4}});
		Morpheus B_2x2 ({{5,7},{1, 2}});

		Morpheus C_2x1 { {4}, {6} };
		Morpheus D_1x2 = { {1,3} };
		Morpheus E_random(10, 10);
		Morpheus prod(1,1);

		std::cout << "A_2x2\n"  << A_2x2 << "\n";
		std::cout << "B_2x2\n"  << B_2x2 << "\n";
		std::cout << "C_2x1 \n" << C_2x1  << "\n";
		std::cout << "D_1x2 \n" << D_1x2  << "\n";

		std::cout << "A_2x2 += B_2x2\n" << A_2x2 + B_2x2 << "\n";
		std::cout << "A_2x2 -= B_2x2\n" << A_2x2 - B_2x2 << "\n";

		std::cout << "A_2x2 *= C_2x1\n" << A_2x2 * C_2x1 << "\n";
		A_2x2 *= 5;
		std::cout << "A_2x2 *= 5\n" << A_2x2 << "\n";

		std::cout << "D_1x2 dot C_2x1\n" << D_1x2.dot(C_2x1) << "\n";

		E_random.fill_random();
		std::cout << E_random << "\n";
		std::cout << E_random.reduced_form() << "\n";
	} catch(std::invalid_argument &ia) {
		std::cout << ia.what();
	}
}
