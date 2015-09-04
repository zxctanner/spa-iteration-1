#include "LineToken.h"
#include "LineTokenizer.h"
#include "ParentTable.h"
#include "ModUseTable.h"
#include "StatementTable.h"
#include "FollowTable.h"
#include "PKB.h"
#include <unordered_map>
#include <stack>

using namespace std;

class Parser {
private:
	PKB* pkb;
	vector<LineToken> tokens;
public:
	/*
	StatementTable st;
	ModUseTable mut;
	ParentTable pt;

	void runParser(vector<LineToken> tokens, vector<pair<int, int>>* parentTable, unordered_map<int, pair<string, string>>* modUseTable,
		unordered_map<int, LineToken*>* statementTable, vector<pair<int, int>>* followTable);
	*/
	Parser();
	Parser(string fileName, PKB* p);
	~Parser();
	void populateFollowTable();
	void populateParentTable();
	void populateStatamentTable();
	void populateModUseTable();
	void populateAllTables();

};