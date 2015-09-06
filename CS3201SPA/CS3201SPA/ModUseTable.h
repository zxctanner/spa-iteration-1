#include <unordered_map>
#include <string>

using namespace std;

class ModUseTable
{
private:
	unordered_map<int, pair<vector<string>, vector<string>>>* modUseTable;
public:
	ModUseTable();
	ModUseTable(unordered_map<int, pair<vector<string>, vector<string>>>* mUTable);
	~ModUseTable();
	unordered_map<int, pair<vector<string>, vector<string>>>* getTable();
	void setModUseTable(unordered_map<int, pair<vector<string>, vector<string>>>* mUTable);
	
	/*
	void insert(int stmtNum, string modified, vector<string> used);
	*/
};

