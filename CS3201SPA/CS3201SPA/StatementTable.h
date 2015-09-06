#ifndef StatementTable_H
#define StatementTable_H
#include "LineToken.h"
#include <unordered_map>

using namespace std;

class StatementTable
{
private:
	unordered_map<int, LineToken*>* statementTable;
public:
	StatementTable();
	StatementTable(unordered_map<int, LineToken*>* sTable);
	~StatementTable();

	unordered_map<int, LineToken*>* getTable();
	void setTable(unordered_map<int, LineToken*>* sTable);
	/*
	void insert(int stmtNum, LineToken);
	LineToken getStmt(int stmtNum);
	*/
};

#endif