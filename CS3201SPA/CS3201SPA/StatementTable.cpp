#include "LineToken.h"
#include "StatementTable.h"
#include <unordered_map>

using namespace std;

StatementTable::StatementTable()
{
	statementTable = new unordered_map<int, LineToken*> ();
}

StatementTable::StatementTable(unordered_map<int, LineToken*>* sTable)
{
	statementTable = sTable;
}

StatementTable::~StatementTable()
{
}

unordered_map<int, LineToken*>* StatementTable::getStatementTable()
{
	return statementTable;
}

void StatementTable::setStatementTable(unordered_map<int, LineToken*>* sTable)
{
	statementTable = sTable;
}

/*
void StatementTable::insert(int stmtNum, LineToken lineToken) {
	StatementTable::table[stmtNum] = &lineToken;
}

LineToken StatementTable::getStmt(int key) {
	try {
		return *StatementTable::table[key];
	}
	catch (exception e) {
		return LineToken();
	}
}
*/
