#include "utils.h"
#include "user_vars.h"
#include "constants.h"
#include "errors.h"

// Initialise global var storage
vector<Variable> vars;

// methods of class Variable
//-----------------------------------------------------------------------------------------------------------------------
double varLookup(vector<Variable> &vars, string s) {
	for (Variable&v : vars) if (v.name == s) return v.val;
	// loop did not find var?
	throw unknown_var{};
};

bool isDeclared(vector<Variable> &vars, string s) { //check if variable named "var" is already in the list
	for (Variable& v : vars) if (v.name == s) return true;
	return false;
};

void declareVar(vector<Variable> &vars, string name, double val, bool mode) { //register new var into list
	if (isDeclared(vars, name)) throw declared_existing_var{};
	vars.push_back(Variable(name, val));
	if (mode) cout << "Assigned value " << val << " to a variable named " << name << ".\n"; //v for verbose
	return;
};

void printVars(vector<Variable> &vars) {
	cout << "Listing all currently registered variables:\n";
	cout << "Name\t\tValue\n";
	for (Variable&v : vars) {
		cout << v.name << "\t\t" << v.val << "\n";
	}
	return;
};
void initConsts() {
	declareVar(vars, "PI", 3.1415926535897932, false);
	declareVar(vars, "E", 2.7182818284590452, false);
	declareVar(vars, "U", 1.660539040e-27, false);
	declareVar(vars, "ME", 9.109e-31, false);
	declareVar(vars, "MN", 1.6749e-27, false);
	declareVar(vars, "MP", 1.6726e-27, false);
}