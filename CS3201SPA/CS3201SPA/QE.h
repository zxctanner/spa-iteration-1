#ifndef QE_H
#define QE_H
#include <vector>
#include <string>
#include "ModUseTable.h"
#include "ParentTable.h"
#include "StatementTable.h"
#include "FollowTable.h"
#include "PKB.h"
#include "Query.h"
#include "QP.h"
using namespace std;

class QE
{
private:
	string filePath;
	PKB* pkb;
public:
	QE();
	QE(string fileName, PKB* p);
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

	vector<string> filter(vector<string> vec, string one, string two);
	vector<string> filter(vector<string> vec, string field);
};

#endif

