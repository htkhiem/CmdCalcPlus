#ifndef _CMDCALCPLUS_CONSTANTS_H_
#define _CMDCALCPLUS_CONSTANTS_H_

using namespace std;
// internal constants, not user facing:
const double pi = 3.1415926535897932; //for rad-to-deg conversion
const double e = 2.7182818284590452; //for hyperbolic trig

// technical squeezes
const char VALUE = 'v';
const char NAME = 'n';
const char DECLARE_VAR_CHAR = 'd';
const string DECLARE_VAR = "LET"; //NOT DECLARE WAR
const char PRINT_VAR_CHAR = 'p';
const string PRINT_VAR = "PRINTVARS";
const char NEWLINE = '\n';
const char ENTER_CHAR = 'e';
const char EXIT = ']';
const char CALCULATE = 'c';
// functions squeezes
const char SINE = '1';		//meaningless magic constants
const char COSINE = '2';	// TODO: use enum
const char TANG = '3';
const char COTG = '4';
const char SINH = '5';
const char COSH = '6';
const char TANH = '7';
const char COTH = '8';
const char SQRT = 's';
const char FLOOR = 'F';
const char CEIL = 'C';
const char RND = 'R';
const char SWITCH_UNIT = 'U';

enum Function {
	fsine, 
	fcosine, 
	ftang, 
	fcotg, 
	fsinh, 
	fcosh, 
	ftanh, 
	fcoth, 
	fsqrt, 
	ffloor, 
	fceil, 
	frnd, 
	fswitchunit
};
enum Command {
	c_user_declare_var,
	c_user_list_vars,
	c_user_enter_key,
	c_exit,
	c_user_calculate
};
#endif