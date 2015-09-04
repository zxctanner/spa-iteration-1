#include <unordered_map>
#include <string>

using namespace std;

class ModUseTable
{
private:
	unordered_map<int, pair<string, vector<string>>>* modUseTable;
public:
	ModUseTable();
	ModUseTable(unordered_map<int, pair<string, vector<string>>>* mUTable);
	~ModUseTable();
	unordered_map<int, pair<string, vector<string>>>* getModUseTable();
	void setModUseTable(unordered_map<int, pair<string, vector<string>>>* mUTable);
	
	/*
	void insert(int stmtNum, string modified, vector<string> used);
	string getLHS(int stmtNum);
	string getRHS(int stmtNum);
	*/
};

