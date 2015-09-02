#include "LineToken.h"
#include "ParentTable.h"
#include "ModUseTable.h"
#include "StatementTable.h"
#include <unordered_map>
#include <stack>

using namespace std;

class Parser {
public:

	StatementTable st;
	ModUseTable mut;
	ParentTable pt;

	void runParser(vector<LineToken> tokens, vector<pair<int, int>>* parentTable, unordered_map<int, pair<string, string>>* modUseTable,
		unordered_map<int, LineToken*>* statementTable, vector<pair<int, int>>* followTable);
};