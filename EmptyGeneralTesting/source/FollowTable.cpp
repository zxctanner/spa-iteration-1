#include "FollowTable.h"
#include <vector>

using namespace std;

FollowTable::FollowTable()
{
	followTable = vector<pair<int, int>>();
}

FollowTable::~FollowTable()
{
}

FollowTable::FollowTable(vector<pair<int, int>> fTable)
{
	followTable = fTable;
}



vector<pair<int, int>> FollowTable::getTable()
{
	return followTable;
}


void FollowTable::setTable(vector<pair<int, int>> fTable)
{
	followTable = fTable;
}



