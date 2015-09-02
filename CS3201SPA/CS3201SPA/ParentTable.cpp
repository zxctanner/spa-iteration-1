#include <vector>

using namespace std;

class ParentTable {
private:
	vector<pair<int,int>> ParentT;
public:
	void addEntry(int parent, int child);
	vector<pair<int,int>> getParentTable();
};

void ParentTable::addEntry(int parent, int child)
{
	pair<int, int> entry(parent,child);
	ParentT.push_back(entry);
}

vector<pair<int,int>> ParentTable::getParentTable()
{
	return ParentT;
}
