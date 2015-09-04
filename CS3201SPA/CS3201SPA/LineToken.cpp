#include "LineToken.h"
#include <string>
using namespace std;

LineToken::LineToken()
{
}

LineToken::LineToken(stmtType tokType, string tokName, int nestLevel, int number, vector<string> rhsexpr)
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

stmtType LineToken::getType()
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

ostream& operator<<(ostream& os, LineToken& token) {
	return os << "LineToken" << " TYPE " << token.getType() 
		<< " NAME " << token.getName() << " LEVEL" << token.getLevel() << " LineNumber " << token.getLevel() << " Expr " << token.getStmtNumber();
}
