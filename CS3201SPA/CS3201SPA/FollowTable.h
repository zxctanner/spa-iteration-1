#include <vector>

using namespace std;

class FollowTable {
private:
	vector<pair<int, int>>* followTable;
public:
	FollowTable();
	~FollowTable();
	FollowTable(vector<pair<int, int>>* fTable);

	//void addEntry(int parent, int child);
	vector<pair<int, int>>* getTable();
	void setTable(vector<pair<int, int>>* fTable);
	//vector<pair<int, int>> generateFollowTable(int nesting[]);
};

