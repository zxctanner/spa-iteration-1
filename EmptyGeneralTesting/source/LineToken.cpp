#include "LineToken.h"
#include <string>
using namespace std;

LineToken::LineToken()
{
}
/*
LineToken::LineToken(stmtType tokType, string tokName, int nestLevel, int number, vector<string> rhsexpr)
{
	type = tokType;
	name = tokName;
	level = nestLevel;
	stmtNumber = number;
	expr = rhsexpr;
}
*/

LineToken::LineToken(string tokType, string tokName, int nestLevel, int number, vector<string> rhsexpr)
{
	type = tokType;
	name = tokName;
	level = nestLevel;
	stmtNumber = number;
	expr = rhsexpr;
}

LineToken::~LineToken()
{
}

/*
stmtType LineToken::getType()
{
	return type;
}
*/
string LineToken::getType()
{
	return type;
}
string LineToken::getName()
{
	return name;
}

int LineToken::getLevel()
{
	return level;
}

int LineToken::getStmtNumber()
{
	return stmtNumber;
}

vector<string> LineToken::getExpr()
{
	return expr;
}


string LineToken::rhsToStr() {
	string str;
	for (auto iter = expr.begin(); iter != expr.end(); iter++) {
		str += *iter;
		str += " ";
	}
	return str;
}
ostream& operator<<(ostream& os, LineToken& token) {
	return os << "LineToken" << " TYPE " << token.getType() 
		<< " NAME " << token.getName() << " LEVEL" << token.getLevel() << " LineNumber " << token.getStmtNumber() << " RHS " << token.rhsToStr();
	
}

string LineToken::getTokenInfo() {
	//return type + " " + name + " " + level + " " + stmtNumber + " " + rhsToStr();
	return "sasd";
}
