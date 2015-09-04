#include <unordered_map>
#include <string>

using namespace std;

class ModUseTable
{
public:
	unordered_map<int, pair<string, vector<string>>> getModUseTable();
	void insert(int stmtNum, string modified, vector<string> used);
	string getLHS(int stmtNum);
	string getRHS(int stmtNum);
};

