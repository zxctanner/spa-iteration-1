#include <string>;
#include <unordered_map>;

using namespace std;

class StatementTable
{
public:
	unordered_map<int, LineToken*> getStmtTable();
	void insert(int stmtNum, LineToken);
	LineToken getStmt(int stmtNum);
};

