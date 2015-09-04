#pragma once
#include <string>
#include <iostream>
#include <vector>
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
	vector<string> expr;

public:
	LineToken();
	LineToken(stmtType tokType, string tokName, int nestLevel, int number, vector<string> rhsepr = {});
	~LineToken();
	stmtType getType();
	string getName();
	int getLevel();
	int getStmtNumber();
	vector<string> getExpr();
	friend ostream& operator<<(ostream& os, LineToken& token);

};

