#ifndef ModUseTable_H
#define ModUseTable_H

#include <unordered_map>
#include <string>
#include <vector>
#include <set>

using namespace std;

class ModUseTable
{
private:
	unordered_map<int, pair<vector<string>, vector<string>>> modUseTable;
	set<string> varList;
public:
	ModUseTable();
	ModUseTable(unordered_map<int, pair<vector<string>, vector<string>>> mUTable);
	ModUseTable(unordered_map<int, pair<vector<string>, vector<string>>> mUTable, set<string> varLst);
	~ModUseTable();
	unordered_map<int, pair<vector<string>, vector<string>>> getTable();
	void setModUseTable(unordered_map<int, pair<vector<string>, vector<string>>> mUTable);
	unordered_map<int, pair<vector<string>, vector<string>>> ModUseTable::getTable();
	set<string> getVarList();
	/*
	void insert(int stmtNum, string modified, vector<string> used);
	*/
};

#endif