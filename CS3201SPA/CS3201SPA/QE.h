#pragma once
#include <string>

class QE
{
public:
	vector<int> ModifiesS(string one, string two);
	vector<string> ModifiesS(int one, string two);

	vector<int> UsesS(string one, string two);
	vector<string> UsesS(int one, string two);

	vector<int> Parent(String one, string two);
	vector<int> Parent(int one, string two);
	vector<int> Parent(String one, int two);

	vector<int> ParentT(String one, string two);
	vector<int> ParentT(int one, string two);
	vector<int> ParentT(String one, int two);

	vector<int> Follows(string one, string two);
	vector<int> Follows(int one, string two);
	vector<int> Follows(string one, int two);

	vector<int> FollowsT(string one, int two);
	vector<int> FollowsT(int one, string two);
	vector<int> FollowsT(string one, int two);

	vector<int> pattern(string one, string two);
};

