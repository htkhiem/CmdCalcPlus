#ifndef _CMDCALCPLUS_USRVARS_H_
#define _CMDCALCPLUS_USRVARS_H_
#include "utils.h"
using namespace std;

class Variable {
public:
	string name;
	double val;
	Variable(string name, double val)
		:name(name), val(val) {}
};
double varLookup(vector<Variable> &vars, string s); // Variable vector can get heavy
bool isDeclared(vector<Variable> &vars, string s);
void declareVar(vector<Variable> &vars, string name, double val, bool mode);
void printVars(vector<Variable> &vars);

extern vector<Variable> vars;

// built-in, user-facing constants
void initConsts();
#endif