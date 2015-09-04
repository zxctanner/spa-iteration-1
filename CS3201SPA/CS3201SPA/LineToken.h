#pragma once
#include <string>
#include <iostream>
using namespace std;

enum stmtType{
	PROCEDURE,
	ASSIGN,
	WHILE
};

class LineToken
{
private:
	stmtType type;
	string name;
	int level;
	int stmtNumber;
	string expr;

public:
	LineToken();
	LineToken(stmtType tokType, string tokName, int nestLevel, int number, string rhsepr = "");
	~LineToken();
	stmtType getType();
	string getName();
	int getLevel();
	int getStmtNumber();
	string getExpr();
	friend ostream& operator<<(ostream& os, LineToken& token);

};

