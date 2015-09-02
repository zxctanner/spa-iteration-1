#include <vector>

using namespace std;

class ParentTable {
private:
	vector<pair<int, int>> ParentT;
public:
	void addEntry(int parent, int child);
	vector<pair<int, int>> getParentTable();
};

