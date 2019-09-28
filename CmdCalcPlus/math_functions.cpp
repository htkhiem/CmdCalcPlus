#include "utils.h"
#include "parser.h"
#include "user_vars.h"
#include "constants.h"
#include "math_functions.h"
#include "errors.h"

//-----------------------------------------------------------------------------------------------------------------------
int factorial(double prim) {
	int fac_in, fac_out, i = 0;
	if (prim - floor(prim) != 0) { //dumbass user trying to factorialize a float
		cout << "Warning: factorizing a non-integer number (" << prim << "). Rounding it to closest integer: ";
		prim = round(prim); //now uses native lib
		cout << prim << ".\n";
	}
	fac_in = static_cast<int>(prim); // Silences compiler warnings.
	fac_out = fac_in;
	#pragma omp for
	for (i = fac_in - 1; i > 1; i--) fac_out *= i;
	return fac_out;
}

double trig(char type, double val, char unit) {
	if (unit == 'd') val = (val * pi / 180);
	switch (type) {
	case SINE: return sin(val);
	case COSINE: return cos(val);
	case TANG: return tan(val);
	case COTG: return 1 / tan(val);
	case SINH: return ((pow(e, 2 * val) - 1) / 2 * pow(e, val));
	case COSH: return ((pow(e, 2 * val) + 1) / 2 * pow(e, val));
	case TANH: return ((pow(e, 2 * val) - 1) / (pow(e, 2 * val) + 1));
	case COTH: return ((pow(e, 2 * val) + 1) / (pow(e, 2 * val) - 1));
	default: throw bad_input{};
	}
};
