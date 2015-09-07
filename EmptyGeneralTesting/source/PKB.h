#ifndef PKB_H
#define PKB_H

#include <vector>
#include <unordered_map>
#include "LineToken.h"
#include "ParentTable.h"
#include "StatementTable.h"
#include "ModUseTable.h"
#include "VarList.h"
#include "VarList.h"
#include "FollowTable.h"

using namespace std;

class PKB
{
private:
	ParentTable* parentTable;
	ModUseTable* modUseTable;
	VarList* varList;
	StatementTable* statementTable;
	FollowTable* followTable;
	//Parser parse;
public:
	//void constructAllTables(vector<LineToken> tokens);
	PKB();
	~PKB();

	ParentTable* getParentTable();
	ModUseTable* getmodUseTable();
	VarList* getVarList();
	StatementTable* getStatementTable();
	FollowTable* getFollowTable();
	void setParentTable(ParentTable* pTable);
	void setModUseTable(ModUseTable* mUTable);
	void setVarList(VarList* vList);
	void setStatementTable(StatementTable* stmtTable);
	void setFollowTable(FollowTable* fTable);
};

#endif