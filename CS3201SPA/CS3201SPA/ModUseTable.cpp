#include "ModUseTable.h"
#include <unordered_map>
#include <string>

using namespace std;


ModUseTable::ModUseTable()
{
	modUseTable = new unordered_map<int, pair<vector<string>, vector<string>>>();
}

ModUseTable::ModUseTable(unordered_map<int, pair<vector<string>, vector<string>>>* mUTable)
{
	modUseTable = mUTable;
}

ModUseTable::~ModUseTable()
{
}

unordered_map<int, pair<vector<string>, vector<string>>>* ModUseTable::getModUseTable() {
	return modUseTable;
}

void ModUseTable::setModUseTable(unordered_map<int, pair<vector<string>, vector<string>>>* mUTable)
{
	modUseTable = mUTable;
}

/*
void ModUseTable::insert(int stmtNum, string modified, vector<string> used) {
	pair<string, vector<string>> assignment(modified, used);
	table[stmtNum] = assignment;
}

string ModUseTable::getLHS(int stmtNum) {
	pair<string, string> assignment;
	try {
		assignment = ModUseTable::table[stmtNum];
	}
	catch (exception e) {
		return NULL;
	}
	return assignment.first;
}

string ModUseTable::getRHS(int stmtNum) {
	pair<string, string> assignment;
	try {
		assignment = ModUseTable::table[stmtNum];
	}
	catch (exception e) {
		return NULL;
	}
	return assignment.second;
}
*/