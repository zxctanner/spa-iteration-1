#include <vector>
#include <unordered_map>
#include "LineToken.h"
#include "Parser.h"
#include "PKB.h"

using namespace std;

/*
void PKB::constructAllTables(vector<LineToken> tokens)
{
	parse.runParser(tokens, &parentTable, &modUseTable, &statementTable, &followTable);
}
*/

PKB::PKB()
{
	parentTable = new ParentTable();
	modUseTable = new ModUseTable();
	varList = new VarList();
	statementTable = new StatementTable();
	followTable = new FollowTable();
}

PKB::~PKB()
{
}


ParentTable* PKB::getParentTable()
{
	return parentTable;
}


ModUseTable* PKB::getmodUseTable()
{
	return modUseTable;
}

VarList* PKB::getVarList() 
{
	return varList;
}

StatementTable* PKB::getStatementTable()
{
	return statementTable;
}

FollowTable* PKB::getFollowTable()
{
	return followTable;
}

void PKB::setParentTable(ParentTable* pTable)
{
	parentTable = pTable;
}

void PKB::setModUseTable(ModUseTable* mUTable)
{
	modUseTable = mUTable;
}

void PKB::setVarList(VarList* vList)
{
	varList = vList;
}

void PKB::setStatementTable(StatementTable* stmtTable)
{
	statementTable = stmtTable;
}

void PKB::setFollowTable(FollowTable* fTable)
{
	followTable = fTable;
}

