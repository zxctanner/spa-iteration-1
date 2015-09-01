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
	int level = 0;
	int stmtNumber = 0;

public:
	LineToken();
	LineToken(stmtType tokType, string tokName, int nestLevel, int number);
	~LineToken();
	stmtType getType();
	string getName();
	int getLevel();
	int getStmtNumber();

};

