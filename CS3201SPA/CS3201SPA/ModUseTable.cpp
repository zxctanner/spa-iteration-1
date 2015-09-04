#include <unordered_map>
#include <string>

using namespace std;

class ModUseTable {
	unordered_map<int, pair<string, vector<string>>> table;

public:
	unordered_map<int, pair<string, vector<string>>> getModUseTable();
	void insert(int stmtNum, string modified, vector<string> used);
	string getLHS(int stmtNum);
	string getRHS(int stmtNum);
};

unordered_map<int, pair<string, vector<string>>> ModUseTable::getModUseTable() {
	return table;
}

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