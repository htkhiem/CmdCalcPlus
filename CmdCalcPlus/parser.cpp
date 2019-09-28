#include "utils.h"
#include "parser.h"
#include "user_vars.h"
#include "math_functions.h"
#include "constants.h"
#include "errors.h"

//-----------------------------------------------------------------------------------------------------------------------
// methods for token streams
//-----------------------------------------------------------------------------------------------------------------------

// Defaults to radians for trigonometric functions.
char unit = 'r'; //d for degrees, r for radians

// Definition of class Token and TokenStream and their methods have been moved to var_functions.cpp.
TokenStream tstream;

Token TokenStream::get() {
	if (buffer_full) { //get Token from buffer instead of istream
		buffer_full = false;
		return buffer;
	}
	//else get from istream as normal
	char kind;
	kind = cin.get();
	while (kind == ' ') kind = cin.get(); //skip leading whitespaces
	switch (kind) {
	case '+': case '-': case '*': case '/': case '(': case ')': case '=': case '!': case '%': case ';': case '^': {
		return Token(kind); //returns a type 1 Token
	}
	case EXIT: return Token(EXIT);
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': {
		cin.putback(kind);
		double val;
		cin >> val;
		return Token(VALUE, val); //returns a type 2 Token
	}
	case NEWLINE: { return Token(ENTER_CHAR); }
	default: {
		if (isalpha(kind)) {
			string s;
			kind = toupper(kind);
			s += kind;
			while (cin.get(kind) && (isalpha(kind) || isdigit(kind))) s += toupper(kind);
			cin.putback(kind);
			if (s == DECLARE_VAR) return Token(DECLARE_VAR_CHAR);
			if (s == PRINT_VAR) return Token(PRINT_VAR_CHAR);
			if ((s == "SIN") || (s == "SINE")) return Token(SINE);
			if ((s == "COS") || (s == "COSINE")) return Token(COSINE);
			if ((s == "TAN") || (s == "TANG") || (s == "TG")) return Token(TANG);
			if ((s == "COTAN") || (s == "COTANG") || (s == "COTG")) return Token(COTG);
			if (s == "SINH") return Token(SINH);
			if (s == "COSH") return Token(COSH);
			if (s == "TANH") return Token(TANH);
			if (s == "COTH") return Token(COTH);
			if ((s == "SQUAREROOT") || (s == "SQRT") || (s == "ROOT")) return Token(SQRT);
			if (s == "FLOOR") return Token(FLOOR);
			if (s == "CEIL") return Token(CEIL);
			if ((s == "ROUND") || (s == "RND")) return Token(RND);
			if (s == "SWITCH") return Token(SWITCH_UNIT);

			return Token(NAME, s); //returns a type 3 token for variable name
		}
		//above if-statement did not return? That's bad input
		throw bad_input{};
	}
	}
}
void TokenStream::putback(Token t) {
	if (buffer_full) throw buffer_overflow{};
	buffer = t;
	buffer_full = true;
	void cleanup();
}
//parsers
//-----------------------------------------------------------------------------------------------------------------------
 //predeclared here so primary() can call it

void switch_unit() { // Switches between deg and rad. char unit is declared in constants.h.
	if (unit == 'd') {
		unit = 'r';
		cout << "Calculator set to radians.\n";
	}
	else {
		unit = 'd';
		cout << "Calculator set to degrees.\n";
	}
	return;
}

double declaration() {
	Token t = tstream.get();
	if (t.kind != NAME) throw invalid_var_declaration{};
	string var_name = t.name;
	Token t2 = tstream.get(); //eats the equal sign too so expression() can proceed normally
	if (t2.kind != '=') throw invalid_var_declaration{};
	double d = expression();
	declareVar(vars, var_name, d, true); //declare_var will throw an exception in case of a re-declaration
	tstream.cleanup();
	return d;
};

double primary() { //parse token stream for either numbers or (expression)
	Token t = tstream.get();
	switch (t.kind) {
	case ('('): { //handle (expression)
		double exp_parenthesis; exp_parenthesis = expression(); //also makes istream skip to after the expression
		t = tstream.get();
		if (t.kind != ')') throw uncoupled_parenthesis{};
		return exp_parenthesis; //returns expression as primary
	}
	case VALUE: {//handle normal floating-point primaries and factorials
		Token t_next = tstream.get();
		if (t_next.kind == '!') return factorial(t.val); //handle factorials
		if (t_next.kind == '^') { //handle powers
			double pow_of = expression();
			return pow(t.val, pow_of);
		}
		else { //just a plain number
			tstream.putback(t_next);
			return t.val; //if token is a number, return its value as a float
		}
	}
	case ('+'): return primary();	//unary plus, simply return next primary as positive
	case ('-'): return -primary();	//same for unary minus
	case SINE: case COSINE: case TANG: case COTG: case SINH: case COSH: case TANH: case COTH: {
		double param = expression();
		return trig(t.kind, param, unit);
	}
	case SQRT: {
		double exp = expression();
		if (exp < 0) throw sqrt_neg{};
		return sqrt(exp);
	}
	case FLOOR: return floor(expression());
	case CEIL: return ceil(expression());
	case RND: return round(expression());
	case NAME: return varLookup(vars, t.name);

	default: throw primary_expected{};
	}
};

double term() {
	double lval = primary();
	Token t = tstream.get();
	while (true) {
		switch (t.kind) {
		case ('*'):
			lval *= primary();
			t = tstream.get(); //read foward, keep loop running
			break;
		case ('/'): {//checks if next primary is zero. if so, throw exception
			double next_term = term();
			if (next_term == 0) throw divide_by_0{};
			lval /= next_term;
			t = tstream.get();
			break;
		}
		case ('%'): {//handle modulus operations, even for floats
			double rval = primary();
			if (lval - floor(lval) != 0) cout << "Warning: Modulus operation on non-integer number: " << lval << "\n";
			if (rval - floor(rval) != 0) cout << "Warning: Modulus operation on non-integer number: " << rval << "\n";
			lval = fmod(lval, rval);
			t = tstream.get();
			break;
		}
		default: {
			tstream.putback(t);
			return lval;
		}
		}
	}
};

double expression() {
	double lval = term();
	Token t = tstream.get();
	while (true) {
		switch (t.kind) {
		case ('+'):
			lval += term(); //get next term so that expression + term = new expression
			t = tstream.get();
			break;
		case ('-'):
			lval -= term(); //same as above for subtracting
			t = tstream.get();
			break;
		default: { //not + or - => this expression has ended => return calculated value of this expression
			tstream.putback(t);
			return lval;
		}
		}
	}
};

//simple cleanup function that cleans istream and token buffer
void TokenStream::cleanup() {
	if (buffer_full) {
		cin.clear();
		buffer_full = false;
	}
	return;
};


void cal() {
	double ans = 0.0;
	char last_cmd = 'i'; //for idle
	while (true) { //loop+catch makes sure program won't close after throwing a recoverable exception
		if (last_cmd != CALCULATE) cout << ">> "; //prints this only after other output. Expression output takes care of itself
		Token t = tstream.get();
		try {
			switch (t.kind) {
			case EXIT: return;
			case DECLARE_VAR_CHAR:
				declaration();
				last_cmd = DECLARE_VAR_CHAR;
				break;
			case PRINT_VAR_CHAR:
				printVars(vars);
				last_cmd = PRINT_VAR_CHAR;
				break;
			case SWITCH_UNIT: {
				switch_unit();
			}
			case ENTER_CHAR:
				if (last_cmd == CALCULATE) {
					cout << "Ans = " << ans << '\n'; //only print this when outputting calculation result
					cout << ">> ";
				}
				tstream.cleanup();
				break;
			default:
				tstream.putback(t);
				ans = expression();
				last_cmd = CALCULATE;
				break;
			}
		}
		catch (divide_by_0) {
			cerr << "Error: Tried to divide by zero.\n";
			tstream.cleanup();
		}
		catch (bad_input) {
			cerr << "Error: Invalid input. If you want to exit, type " << EXIT << '.\n';
			tstream.cleanup();
		}
		catch (uncoupled_parenthesis) {
			cerr << "Error: A parenthesis was not closed.\n";
			tstream.cleanup();
		}
		catch (primary_expected) {
			cerr << "Error: Expected a primary.\n";
			tstream.cleanup();
		}
		catch (declared_existing_var) {
			cerr << "Error: A variable with this name has already been declared.\n";
			tstream.cleanup();
		}
		catch (invalid_var_declaration) {
			cerr << "Error: Invalid variable declaration syntax.\n";
			tstream.cleanup();
		}
		catch (sqrt_neg) {
			cerr << "Error: Tried to take square root of a negative number.\n";
			tstream.cleanup();
		}
		catch (unknown_var) {
			cerr << "Error: Unknown variable name.\n";
			tstream.cleanup();
		}
	}
};