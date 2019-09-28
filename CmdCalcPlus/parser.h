#ifndef _CMDCALCPLUS_PARSER_H_
#define _CMDCALCPLUS_PARSER_H_
#include "utils.h"
#include "user_vars.h"
using namespace std;

class Token {
public:
	char kind;
	double val;
	string name;
	//constructor:
	Token(char ch): //type 1: operators, brackets, ...
		kind(ch), val(0.0) {}
	Token(char ch, double num): //type 2: floating point numbers
		kind(ch), val(num) {}
	Token(char ch, string s): //type 3: variable names
		kind(ch), name(s), val(0.0) {}
};

class TokenStream {
public:
	TokenStream():
		buffer_full(false), buffer(0) {}
	Token get();
	void putback(Token t);
	void cleanup();
private:
	Token buffer; //stream is actually a single slot buffer lol
	bool buffer_full;
};

extern TokenStream tstream;
extern char unit;

void switch_unit();
double declaration();
double expression();
double primary();
double term();
void cal();

#endif

