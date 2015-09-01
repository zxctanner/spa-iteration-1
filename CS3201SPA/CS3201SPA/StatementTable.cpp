#include "StatementTable.h"
#include "LineToken.h"
#include <unordered_map>

using namespace std;

class StatementTable {
	unordered_map<int, LineToken*> table;

public:
	void insert(int stmtNum, LineToken);
	LineToken getStmt(int stmtNum);
};

StatementTable::StatementTable()
{

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
