#pragma once
#include <string>

class QE
{
public:
	int Modifies(string first, string second);
	string Modifies(int first, string second);

	int Uses(string first, string second);
	string Uses(int first, string second);

	int Parent(int first, int second);
	int ParentS(int first, int second);

	int Follows(string first, int second);
	int Follows(int first, string second);
	int FollowsS(string first, int second);
	int FollowsS(int first, string second);
};

