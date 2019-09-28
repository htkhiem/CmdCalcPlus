#include "utils.h"
#include "user_vars.h"
#include "parser.h"
#include "constants.h"
#include "math_functions.h"
#include "errors.h"

int main() {
	cout << "Simple calculator v1.2.1. Accepts input like this: 2.1+4.006/3*((7%8e-1)+5!). After typing, hit Enter to calculate.\n";
	cout << "End the program by entering " << EXIT << ".\n";
	cout << "Default mode: radians. To switch to degrees, type 'switch'.\n";
	try {  //catches fatal exceptions
		cal();
	}
	catch (buffer_overflow) {
		cerr << "Error: Tried writing to full buffer.\n";
		keep_window_open();
		return 1;
	}
	catch(exception& e) {
		cerr << "Error: " << e.what() << "\n.";
		keep_window_open();
		return 1;
	}
	catch (...) {
		cerr << "Unknown exception. ";
		keep_window_open();
		return 2;
	}
}