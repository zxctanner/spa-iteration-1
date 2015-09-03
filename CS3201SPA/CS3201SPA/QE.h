#pragma once
#include <string>

class QE
{
public:
	vector<string> ModifiesS(string one, string two);
	vector<string> ModifiesS(int one, string two);

	vector<string> UsesS(string one, string two);
	vector<string> UsesS(int one, string two);

	vector<string> Parent(string one, string two);
	vector<string> Parent(int one, string two);
	vector<string> Parent(string one, int two);

	vector<string> ParentT(string one, string two);
	vector<string> ParentT(int one, string two);
	vector<string> ParentT(string one, int two);

	vector<string> Follows(string one, string two);
	vector<string> Follows(int one, string two);
	vector<string> Follows(string one, int two);

	vector<string> FollowsT(string one, string two);
	vector<string> FollowsT(int one, string two);
	vector<string> FollowsT(string one, int two);

	vector<string> pattern(string one, string two);
};

