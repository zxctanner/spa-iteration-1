
#include <vector>

using namespace std;

class FollowTable {
private:
	vector<pair<int, int>> FollowT;
public:
	void addEntry(int parent, int child);
	vector<pair<int, int>> getFollowTable();
	vector<pair<int, int>> generateFollowTable(int nesting[]);
};

void FollowTable::addEntry(int parent, int child)
{
	pair<int, int> entry(parent, child);
	FollowT.push_back(entry);
}

vector<pair<int, int>> FollowTable::getFollowTable()
{
	return FollowT;
}

/*
vector<pair<int, int>> FollowTable::generateFollowTable(int nesting[])
{
	// pass in a nesting table
	// int nesting[] = { 0,1,2,3,3,2,0 };

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

				pair<int, int> entry(n+1, m+1);
				FollowT.push_back(entry);
			}

		}
	}

	return FollowT;
}
*/
