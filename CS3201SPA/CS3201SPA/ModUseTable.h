#include <string>;
#include <unordered_map>;

using namespace std;

class ModUseTable
{
public:
	unordered_map <int, pair<string, string>> getModUseTable();
	void insert(int stmtNum, string modified, string used);
	string getLHS(int stmtNum);
	string getRHS(int stmtNum);
};

