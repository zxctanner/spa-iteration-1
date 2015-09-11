#include "LineToken.h"
#include "StatementTable.h"
#include <unordered_map>

using namespace std;

StatementTable::StatementTable()
{
	statementTable = unordered_map<int, LineToken> ();
}

StatementTable::StatementTable(unordered_map<int, LineToken> sTable)
{
	statementTable = sTable;
}

StatementTable::~StatementTable()
{
}

unordered_map<int, LineToken> StatementTable::getTable()
{
	return statementTable;
}

void StatementTable::setTable(unordered_map<int, LineToken> sTable)
{
	statementTable = sTable;
}


