#include "StatementTable.h"
#include "LineToken.h"
#include <unordered_map>

using namespace std;

class StatementTable {
	unordered_map<int, LineToken*> table;

public:
	unordered_map<int, LineToken*> getStmtTable();
	void insert(int stmtNum, LineToken);
	LineToken getStmt(int stmtNum);
};

unordered_map<int, LineToken*> StatementTable::getStmtTable()
{
	return table;
}

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
