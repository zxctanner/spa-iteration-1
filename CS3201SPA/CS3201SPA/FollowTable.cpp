#include "FollowTable.h"
#include <vector>

using namespace std;

FollowTable::FollowTable()
{
	followTable = new vector<pair<int, int>>();
}

FollowTable::~FollowTable()
{
}

FollowTable::FollowTable(vector<pair<int, int>>* fTable)
{
	followTable = fTable;
}

/*
void FollowTable::addEntry(int parent, int child)
{
	pair<int, int> entry(parent, child);
	followTable->push_back(entry);
}
*/

vector<pair<int, int>>* FollowTable::getFollowTable()
{
	return followTable;
}


void FollowTable::setFollowTable(vector<pair<int, int>>* fTable)
{
	followTable = fTable;
}



