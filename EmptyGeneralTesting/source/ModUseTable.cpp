#include "ModUseTable.h"


using namespace std;


ModUseTable::ModUseTable()
{
	modUseTable = unordered_map<int, pair<vector<string>, vector<string>>>();
}

ModUseTable::ModUseTable(unordered_map<int, pair<vector<string>, vector<string>>> mUTable)
{
	modUseTable = mUTable;
}

ModUseTable::ModUseTable(unordered_map<int, pair<vector<string>, vector<string>>> mUTable, set<string> varLst)
{
	modUseTable = mUTable;
	varList = varLst;
}

ModUseTable::~ModUseTable()
{
}

unordered_map<int, pair<vector<string>, vector<string>>> ModUseTable::getTable() {
	return modUseTable;
}

set<string> ModUseTable::getVarList()
{
	return varList;
}

void ModUseTable::setModUseTable(unordered_map<int, pair<vector<string>, vector<string>>> mUTable)
{
	modUseTable = mUTable;
}

/*
void ModUseTable::insert(int stmtNum, string modified, vector<string> used) {
	pair<string, vector<string>> assignment(modified, used);
	table[stmtNum] = assignment;
}
*/