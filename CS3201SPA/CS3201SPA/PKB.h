#include <vector>
#include <unordered_map>
#include "LineToken.h"
#include "Parser.h"

using namespace std;

class PKB
{
private:
	vector<pair<int, int>> parentTable;
	unordered_map<int, pair<string, string>> modUseTable;
	unordered_map<int, LineToken*> statementTable;
	vector<pair<int, int>> followTable;
	Parser parse;
public:
	void constructAllTables(vector<LineToken> tokens);
	vector<pair<int, int>> getParentTable();
	unordered_map<int, pair<string, string>> getmodUseTable();
	unordered_map<int, LineToken*> getStatementTable();
	vector<pair<int, int>> getFollowTable();
};
