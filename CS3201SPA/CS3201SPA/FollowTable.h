#include <vector>

using namespace std;

class FollowTable {
private:
	vector<pair<int, int>> FollowT;
public:
	void addEntry(int parent, int child);
	vector<pair<int, int>> getFollowTable();
	vector<pair<int, int>> generateFollowTable();
};

