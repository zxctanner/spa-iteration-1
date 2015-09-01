#include "LineToken.h"
#include <string>
using namespace std;

LineToken::LineToken()
{
}

LineToken::LineToken(stmtType tokType, string tokName, int nestLevel, int number)
{
	type = tokType;
	name = tokName;
	level = nestLevel;
	stmtNumber = number;

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
