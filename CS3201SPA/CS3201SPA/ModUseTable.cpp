#include <unordered_map>
#include <string>

using namespace std;

class ModUseTable {
	unordered_map<int, pair<string, string>> table;

public:
	unordered_map<int, pair<string, string>> getModUseTable();
	void insert(int stmtNum, string modified, string used);
	string getLHS(int stmtNum);
	string getRHS(int stmtNum);
};

unordered_map<int, pair<string, string>> ModUseTable::getModUseTable() {
	return table;
}

void ModUseTable::insert(int stmtNum, string modified, string used) {
	pair<string, string> assignment(modified, used);
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