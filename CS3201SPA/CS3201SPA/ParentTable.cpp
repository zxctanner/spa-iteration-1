#include "ParentTable.h"
#include <vector>

using namespace std;

ParentTable::ParentTable()
{
	parentTable = new vector<pair<int, int>>();
}

ParentTable::ParentTable(vector<pair<int, int>>* pTable)
{
	parentTable = pTable;
}

ParentTable::~ParentTable()
{
}

void ParentTable::addEntry(int parent, int child)
{
	pair<int, int> entry(parent,child);
	parentTable->push_back(entry);
}

vector<pair<int,int>>* ParentTable::getParentTable()
{
	return parentTable;
}

void ParentTable::setParentTable(vector<pair<int, int>>* pTable)
{
	parentTable = pTable;
}
