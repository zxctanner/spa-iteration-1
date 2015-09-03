
#include <vector>

using namespace std;

class FollowTable {
private:
	vector<pair<int, int>> ParentT;
public:
	void addEntry(int parent, int child);
	vector<pair<int, int>> getFollowTable();
	vector<pair<int, int>> generateFollowTable();
};

void FollowTable::addEntry(int parent, int child)
{
	pair<int, int> entry(parent, child);
	ParentT.push_back(entry);
}

vector<pair<int, int>> FollowTable::getFollowTable()
{
	return ParentT;
}


vector<pair<int, int>> FollowTable::generateFollowTable()
{
	int nesting[] = { 0,1,2,3,3,2,0 };

	// count the size of the nesting array;
	int countSize = sizeof(nesting) / sizeof(*nesting);

	// n for outer loop, n for inner loop
	int n;

	int m;

	for (n = 0; n < countSize - 1; n++) {

		// get the current nesting level
		int currentNesting = nesting[n];

		for (m = n + 1; m < countSize; m++) {

			int nextNesting = nesting[m];

			// if next nesting level drops below current level, no need to proceed
			if (nextNesting < currentNesting) {
				break;
			}

			// if same level means follow
			if (nextNesting == currentNesting) {
				//std::cout << (n + 1) << (m + 1) << "\n";

			}

		}
	}

	return ParentT;
}

