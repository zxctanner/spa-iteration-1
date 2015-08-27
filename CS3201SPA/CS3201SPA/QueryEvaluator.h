#pragma once
class QueryEvaluator
{
public:
	int Modifies(String, String);
	String Modifies(int, String);

	int Uses(String, String);
	String Uses(int, String);

	int Parent(int, int);
	int ParentS(int, int);

	int Follows(String, int);
	int Follows(int, String);
	int FollowsS(String, int);
	int FollowsS(int, String);
};

