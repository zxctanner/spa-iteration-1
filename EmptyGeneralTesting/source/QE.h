#ifndef QE_H
#define QE_H
#include <vector>
#include <string>
#include <list>
#include "ModUseTable.h"
#include "ParentTable.h"
#include "StatementTable.h"
#include "FollowTable.h"
#include "PKB.h"
#include "LineToken.h"
#include "Query.h"
#include "QP.h"
using namespace std;

// token.getType() == ASSIGN
//token.getType() == WHILE

class QE
{
private:
	string filePath;
	string queryStringForQP;
	PKB* pkb;
	vector<string> answers;
	vector<Query> queryVector;
	list<string> answerForSingleQuery;

public:
	QE(PKB* p);
	QE(string fileName, PKB* p);
	
	void getQSForQP(string queryString);
	void evaluateSingleQuery(string queryString);
	list<string> getAnswerForSingleQuery();
	list<string> convertVectorToList(vector<string> answerVec);
	vector<string> selectField(string select, string command, string one, string two, Query q);
	bool isInt(string input);
	vector<string> Choices(string choice, bool status);
	vector<string> ModifiesS(string select, string one, string two, Query q);
	vector<string> ModifiesS(string select, int one, string two, Query q);

	vector<string> UsesS(string select, string one, string two, Query q);
	vector<string> UsesS(string select, int one, string two, Query Q);

	vector<string> Parent(string select, string one, string two, Query q);
	vector<string> Parent(string select, int one, string two, Query q);
	vector<string> Parent(string select, string one, int two, Query q);
	vector<string> Parent(string select, int one, int two, Query q);

	vector<string> ParentT(string select, string one, string two, Query q);
	vector<string> ParentT(string select, int one, string two, Query q);
	vector<string> ParentT(string select, string one, int two, Query q);
	vector<string> ParentT(string select, int one, int two, Query q);

	vector<string> Follows(string select, string one, string two, Query q);
	vector<string> Follows(string select, int one, string two, Query q);
	vector<string> Follows(string select, string one, int two, Query q);
	vector<string> Follows(string select, int one, int two, Query q);

	vector<string> FollowsT(string select, string one, string two, Query q);
	vector<string> FollowsT(string select, int one, string two, Query q);
	vector<string> FollowsT(string select, string one, int two, Query q);

	vector<string> pattern(string select, string command, string one, string two, Query q);

	vector<string> getAllType(string type);
	int relation(string select, string one, string two);
	vector<string> filter(vector<string> vec, string field, Query q);
	vector<string> findCommonAnswer(vector<string> ansST, vector<string> ansP);
	string vectorSToString(vector<string> vecString);
	void displayAllAnswers();
	bool checkAnswerSize(vector<string> answerVector);
};

#endif

