#include "ParentTable.h"
#include <vector>

using namespace std;

ParentTable::ParentTable()
{
	parentTable = vector<pair<int, int>>();
}

ParentTable::ParentTable(vector<pair<int, int>> pTable)
{
	parentTable = pTable;
}

ParentTable::~ParentTable()
{
}

vector<pair<int,int>> ParentTable::getTable()
{
	return parentTable;
}

void ParentTable::setTable(vector<pair<int, int>> pTable)
{
	parentTable = pTable;
}
