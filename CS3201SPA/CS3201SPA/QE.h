#pragma once
#include <string>

class QE
{
public:
	int ModifiesS(string first, string second);
	string ModifiesS(int first, string second);

	int UsesS(string first, string second);
	string UsesS(int first, string second);

	int Parent(String first, string second);
	int Parent(int first, string second);
	int Parent(String first, int second);

	int ParentT(String first, string second);
	int ParentT(int first, string second);
	int ParentT(String first, int second);

	int Follows(string first, string second);
	int Follows(int first, string second);
	int Follows(string first, int second);

	int FollowsT(string first, int second);
	int FollowsT(int first, string second);
	int FollowsT(string first, int second);

	int pattern(string first, string second);
};

