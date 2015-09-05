#pragma once
#include <string>

class QE
{
public:
	vector<string> ModifiesS(string select, string one, string two);
	vector<string> ModifiesS(string select, int one, string two);

	vector<string> UsesS(string select, string one, string two);
	vector<string> UsesS(string select, int one, string two);

	vector<string> Parent(string select, string one, string two);
	vector<string> Parent(string select, int one, string two);
	vector<string> Parent(string select, string one, int two);

	vector<string> ParentT(string select, string one, string two);
	vector<string> ParentT(string select, int one, string two);
	vector<string> ParentT(string select, string one, int two);

	vector<string> Follows(string select, string one, string two);
	vector<string> Follows(string select, int one, string two);
	vector<string> Follows(string select, string one, int two);

	vector<string> FollowsT(string select, string one, string two);
	vector<string> FollowsT(string select, int one, string two);
	vector<string> FollowsT(string select, string one, int two);

	vector<string> pattern(string select, string one, string two);

	vector<string> filter(vector<string> vec, string field);
};

